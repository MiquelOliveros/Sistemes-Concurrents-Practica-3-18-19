/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#include "service.h"
#include "server.h"

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>


#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include<semaphore.h>

// globals
	char g_pwd[PATH_MAX+1];	
	int sesion;
	int maxThreads;
	int threadsInUse;
	pthread_t destroyerTid;
	int serverSocket;
	int fatality;
	
	//variables 3a prac
	sem_t threadsFree;
	
	pthread_cond_t finishSincro;
    pthread_mutex_t mutexQueue;
	circularQueue queue_finish;
	
	pthread_mutex_t mutexStats;
	pthread_mutex_t mutexServerStats;
	TStatsprog serverStats;
	
	pthread_mutex_t controlC;
	pthread_barrier_t end;

// functions
void controlador(int);

void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}

int main(int a_argc, char **ap_argv)
{
	// variables
    int clientSocket;
    socklen_t clientAddrSize;		
	struct sockaddr_in clientAddr;		
	struct sigaction signalAction;

		
	// check args
		if(a_argc < 4)
		{
			printf("Usage: %s dir_name port_number maxThreads\n", ap_argv[0]);
			return 1;
		}
		
	// init vars
		realpath(ap_argv[1], g_pwd);
		maxThreads = atoi(ap_argv[3]);
		                              
		threadsInUse = 0; //XXX s'empra per la barrera del controlC
		sesion = 0;
		fatality = 0;
		
		/*Iniciar Semaforo maximos threds*/
		sem_init(&threadsFree, 0, maxThreads);
		
		/*Iniciar stats*/
		pthread_mutex_init(&mutexStats, NULL);
		pthread_mutex_init(&mutexServerStats, NULL);
		start_Stats(&serverStats);
		
		/*Iniciar cua finalitzats*/
		pthread_cond_init(&finishSincro, NULL); 
		pthread_mutex_init(&mutexQueue, NULL);
		initDefQueue(&queue_finish, maxThreads);
		
		/*Iniciar mutex Ctrl+c*/    
		pthread_mutex_init(&controlC, NULL);
		

	// create service
		if(!service_create(&serverSocket, strtol(ap_argv[2], (char**)NULL, 10)))
		{
			fprintf(stderr, "%s: unable to create service on port: %s\n", ap_argv[0], ap_argv[2]);
			return 2;
		}
		
	// setup termination handler
		signalAction.sa_handler = sigchld_handler; // reap all dead processes
		sigemptyset(&signalAction.sa_mask);
		signalAction.sa_flags = SA_RESTART;
		
		if (sigaction(SIGCHLD, &signalAction, NULL) == -1)
		{
			perror("main(): sigaction");
			return 3;
		}

	//la senyal no ha pogut ser executada
	if(signal(SIGINT,controlador) == SIG_ERR)
	{
		perror("Server could not be closed");
	}
	
	/*Creem un fil que s'encarregara deprocessar les estadistiques i fer el join dels fils*/
	pthread_create(&destroyerTid, NULL, (void *(*) (void *))shine_threads, NULL);
	threadsInUse++;
	
		// dispatcher loop
		while(!fatality)
		{
			clientAddrSize = sizeof(clientAddr);
			
			// wait for a client
			/*Implementar concurrencia*/
			sem_wait(&threadsFree);
			
			#ifndef NOSERVERDEBUG
				printf("\nmain(): waiting for clients...\n");
    		#endif
    		
    		if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize)) != -1)
			{
                sesion++;
                
                TControlTid *controlTid = (TControlTid*) malloc(sizeof(TControlTid));
                
				start_Stats(&(controlTid->stats));	
				controlTid->sesion_id = sesion;
				controlTid->clientSocket = clientSocket;

				#ifndef NOSERVERDEBUG
					printf("\nmain(): got client connection [addr=%s,port=%d] --> %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), sesion);
				#endif
					 
				// dispatch job
				// service the client
				if(session_create(clientSocket))
				{
				    threadsInUse++;
					pthread_create(&controlTid->tid, NULL, (void*(*) (void *))hajimemasho, (void*)controlTid);
				}
				
			pthread_mutex_unlock(&controlC);
			}		
			else{
				perror("main()");
				pthread_mutex_unlock(&controlC);
				sem_post(&threadsFree);
			}
				
		}

		// destroy service
		close(serverSocket);
		return 0;
}

	/*Funció que executen els fils dels clients per respondre les peticions*/
	void* hajimemasho(TControlTid *controlData)
	{
		service_loop(controlData->clientSocket, controlData);
		gettimeofday(&controlData->stats.temps_final, NULL);
		
		pthread_mutex_lock(&mutexStats);        /*LOCK*/
		printf(ANSI_COLOR_YELLOW "\nSESION %d EVENT\n" ANSI_COLOR_RESET, controlData->sesion_id);
		printSesionStats(&controlData->stats);
		pthread_mutex_unlock(&mutexStats);      /*UNLOCK*/
		
		session_destroy(controlData->clientSocket);
		close(controlData->clientSocket); // parent doesn't need this socket								
		
		if(!fatality){
		    pthread_mutex_lock(&mutexQueue);        /*LOCK*/
	        addQueue(&queue_finish, controlData->tid);
	        pthread_cond_signal(&finishSincro);
	        pthread_mutex_unlock(&mutexQueue);      /*UNLOCK*/
		}
		else{
		    pthread_barrier_wait(&end);
		}

		free(controlData);

		pthread_exit(NULL);
	}
	
	/*Funció ques'executa per comprovar si un fild'atenció ha finalitzat o no
	 *Inicia les estadístiques del servidor i va acumulant-les
	 *Deixa l'estructura global preparada per la següent atenció a un client
	*/
	void shine_threads()
	{	
        while(true)
        {
        	if(fatality){
            	pthread_mutex_lock(&mutexQueue);
	            while(queue_finish.size != 0)
                {
                    pthread_t tid = delQueue(&queue_finish);       ///XXX int no suficient per emmagatzemar TID
                    if(pthread_join(tid, NULL) == -1)
                    {
		                perror("Error al finalitzar la petició del client\n");
	                }else{
	                    threadsInUse--;
		                sem_post(&threadsFree);
	                }
                }
                pthread_mutex_unlock(&mutexQueue);
                pthread_barrier_wait(&end);
                pthread_exit(NULL);                                             
        	}else{	
        	    pthread_mutex_lock(&mutexQueue);
	            while(queue_finish.size == 0 && !fatality)
	            {
	                pthread_cond_wait(&finishSincro, &mutexQueue);
	                while(queue_finish.size != 0)
	                {
	                    pthread_t tid = delQueue(&queue_finish);       ///XXX int no suficient per emmagatzemar TID
	                    if(pthread_join(tid, NULL) == -1)
	                    {
			                perror("Error al finalitzar la petició del client\n");
		                }else{
		                    threadsInUse--;
			                sem_post(&threadsFree);
		                }
	                } 
	            }
	            pthread_mutex_unlock(&mutexQueue);
	        }
        }	    		
		pthread_exit(NULL);
	}

Boolean service_create(int *ap_socket, const int a_port)
{
	// variables
		struct sockaddr_in serverAddr;
		
		#ifdef _PLATFORM_SOLARIS
			char yes='1';
		#else
			int yes=1;
		#endif
		
	// create address
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(a_port);
		
	// create socket
		if((*ap_socket = socket(serverAddr.sin_family, SOCK_STREAM, 0)) < 0)
		{
			perror("service_create(): create socket");
			return false;
		}
		
	// set options
		if(setsockopt(*ap_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		{
			perror("service_create(): socket opts");
			return false;
		}
	
	// bind socket
		if(bind(*ap_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			perror("service_create(): bind socket");
			close(*ap_socket);
			return false;
		}
		
	// listen to socket
		if(listen(*ap_socket, SERVER_SOCKET_BACKLOG) < 0)
		{
			perror("service_create(): listen socket");
			close(*ap_socket);
			return false;
		}
	
	return true;
}

Boolean session_create(const int a_socket)
{
	// variables
		Message msgOut, msgIn;
		
	// init vars
		Message_clear(&msgOut);
		Message_clear(&msgIn);
		
	// session challenge dialogue
	
		// client: greeting
			if(!siftp_recv(a_socket, &msgIn) || !Message_hasType(&msgIn, SIFTP_VERBS_SESSION_BEGIN))
			{
				fprintf(stderr, "[%d] service_create(): session not requested by client.\n",sesion);
				return false;
			}
			
		// server: identify
		// client: username
			Message_setType(&msgOut, SIFTP_VERBS_IDENTIFY);
			
			if(!service_query(a_socket, &msgOut, &msgIn) || !Message_hasType(&msgIn, SIFTP_VERBS_USERNAME))
			{
				fprintf(stderr, "[%d] service_create(): username not specified by client.\n",sesion);
				return false;
			}
		
		// server: accept|deny
		// client: password
			Message_setType(&msgOut, SIFTP_VERBS_ACCEPTED); //XXX check username... not required for this project
			
			if(!service_query(a_socket, &msgOut, &msgIn) || !Message_hasType(&msgIn, SIFTP_VERBS_PASSWORD))
			{
				fprintf(stderr, "[%d] service_create(): password not specified by client.\n",sesion);
				return false;
			}
		
		// server: accept|deny
			if(Message_hasValue(&msgIn, SERVER_PASSWORD))
			{
				Message_setType(&msgOut, SIFTP_VERBS_ACCEPTED);
				siftp_send(a_socket, &msgOut);
			}
			else
			{
				Message_setType(&msgOut, SIFTP_VERBS_DENIED);
				siftp_send(a_socket, &msgOut);
				
				fprintf(stderr, "[%d] service_create(): client password rejected.\n",sesion);
				return false;
			}
		
		// session now established
			#ifndef NOSERVERDEBUG
				printf("[%d] session_create(): success\n",sesion);
			#endif
			
	return true;
}
/*Afegir punter a laestructura de dades de stats*/
//TODO
void service_loop(const int a_socket, TControlTid *controlData)
{
	// variables
		Message msg;
		
		String *p_argv;
		int argc;
		
	// init vars
		Message_clear(&msg);
	
	while(siftp_recv(a_socket, &msg)) // await request
	{
		if(Message_hasType(&msg, SIFTP_VERBS_SESSION_END) || Message_hasType(&msg, ""))
			break;
		
		else
		{
			#ifndef NOSERVERDEBUG
				printf("[%d] service_loop(): got command '%s'\n",controlData->sesion_id, Message_getValue(&msg));
			#endif
			
			// parse request
				if((p_argv = service_parseArgs(Message_getValue(&msg), &argc)) == NULL || argc <= 0)
				{
					service_freeArgs(p_argv, argc);
					
					if(!service_sendStatus(a_socket, false)) // send negative ack
						break;
					
					continue;
				}
				
			// handle request
				if(!service_handleCmd(a_socket, p_argv, argc, controlData))
					service_sendStatus(a_socket, false); // send negative ack upon fail
				
			// clean up
				service_freeArgs(p_argv, argc);
				p_argv = NULL;
				
				/*
				if(!service_handleCmd(a_socket, Message_getValue(&msg))) // send failure notification
				{
					Message_setType(&msg, SIFTP_VERBS_ABORT);
					
					if(!siftp_send(a_socket, &msg))
					{
						fprintf(stderr, "service_loop(): unable to send faliure notice.\n");
						break;
					}
				}
				*/
		}
	}
	/*modificar aqui siha acabat el thread o no*/
}
/*Afegir punter a la estructura de dades de stats*/
//TODO
Boolean service_handleCmd(const int a_socket, const String *ap_argv, const int a_argc, TControlTid *controlData)
{
	// variables
		Message msg;
		
		String dataBuf;
		int dataBufLen;
		
		Boolean tempStatus = false;
		
	// init variables
		Message_clear(&msg);
		
	// handle commands
	if(strcmp(ap_argv[0], "ls") == 0)
	{
		if((dataBuf = service_readDir(g_pwd, &dataBufLen)) != NULL)
		{
			// transmit data
				if(service_sendStatus(a_socket, true))
					tempStatus = siftp_sendData(a_socket, dataBuf, dataBufLen);
				
				#ifndef NOSERVERDEBUG
					printf("[%d] ls(): status=%d\n",controlData->sesion_id, tempStatus);
					
					controlData->stats.comandes += 1;
					
					pthread_mutex_lock(&mutexServerStats);
					gettimeofday(&serverStats.temps_final, NULL);
					incrementarComandes(&serverStats);
					pthread_mutex_unlock(&mutexServerStats);
					
				#endif
				
			// clean up
				free(dataBuf);
				
			return tempStatus;
		}
	}
	
	else if(strcmp(ap_argv[0], "pwd") == 0)
	{
		if(service_sendStatus(a_socket, true)){
			controlData->stats.comandes += 1;
			
			pthread_mutex_lock(&mutexServerStats);
			gettimeofday(&serverStats.temps_final, NULL);
			incrementarComandes(&serverStats);
			pthread_mutex_unlock(&mutexServerStats);
			
			return siftp_sendData(a_socket, g_pwd, strlen(g_pwd));
		}
	}
	
	else if(strcmp(ap_argv[0], "cd") == 0 && a_argc > 1)
	{
		controlData->stats.comandes += 1;
		
		pthread_mutex_lock(&mutexServerStats);
		gettimeofday(&serverStats.temps_final, NULL);
		incrementarComandes(&serverStats);
		pthread_mutex_unlock(&mutexServerStats);
		
		return service_sendStatus(a_socket, service_handleCmd_chdir(g_pwd, ap_argv[1]));
	}
	
	else if(strcmp(ap_argv[0], "get") == 0 && a_argc > 1)
	{
		char srcPath[PATH_MAX+1];
		
		// determine absolute path
		if(service_getAbsolutePath(g_pwd, ap_argv[1], srcPath))
		{
			// check read perms & file type
			if(service_permTest(srcPath, SERVICE_PERMS_READ_TEST) && service_statTest(srcPath, S_IFMT, S_IFREG))
			{
				// read file
				if((dataBuf = service_readFile(srcPath, &dataBufLen)) != NULL)
				{
					if(service_sendStatus(a_socket, true))
					{
						// send file
						struct timeval temps_inicial, temps_final;
						
						gettimeofday(&temps_inicial,NULL);
						tempStatus = siftp_sendData(a_socket, dataBuf, dataBufLen);
						gettimeofday(&temps_final,NULL);
						float incMb = (float)dataBufLen/(1024*1024);
						double incTemps = (double) (temps_final.tv_sec - temps_inicial.tv_sec) 
						                            + ((temps_final.tv_usec - temps_inicial.tv_usec)/1000000.0);
						                            
						incrementarTransferencia(&controlData->stats.mbGet, incMb, &controlData->stats.tempsGet, 
						                            incTemps, &controlData->stats.numGet);
						controlData->stats.comandes += 1;
						
		                pthread_mutex_lock(&mutexServerStats);
		                gettimeofday(&serverStats.temps_final, NULL);
		                incrementarTransferencia(&serverStats.mbGet, incMb, &serverStats.tempsGet, 
						                            incTemps, &serverStats.numGet);
						incrementarComandes(&serverStats);
		                pthread_mutex_unlock(&mutexServerStats);
		                
						#ifndef NOSERVERDEBUG
							printf("[%d] get(): file sent %s.\n",controlData->sesion_id, tempStatus ? "OK" : "FAILED");
						#endif
					}
					#ifndef NOSERVERDEBUG
					else
						printf("[%d] get(): remote host didn't get status ACK.\n",controlData->sesion_id);
					#endif
					
					free(dataBuf);
				}
				#ifndef NOSERVERDEBUG
				else
					printf("[%d]get(): file reading failed.\n",controlData->sesion_id);
				#endif
			}
			#ifndef NOSERVERDEBUG
			else
				printf("[%d]get(): don't have read permissions.\n",controlData->sesion_id);
			#endif
		}
		#ifndef NOSERVERDEBUG
		else
			printf("[%d] get(): absolute path determining failed.\n",controlData->sesion_id);
		#endif
			
		return tempStatus;
	}
	
	else if(strcmp(ap_argv[0], "put") == 0 && a_argc > 1)
	{
		char dstPath[PATH_MAX+1];
		
		// determine destination file path
		if(service_getAbsolutePath(g_pwd, ap_argv[1], dstPath))
		{
			// check write perms & file type
			if(service_permTest(dstPath, SERVICE_PERMS_WRITE_TEST) && service_statTest(dstPath, S_IFMT, S_IFREG))
			{
				// send primary ack: file perms OK
				if(service_sendStatus(a_socket, true))
				{
					// receive file
					if((dataBuf = siftp_recvData(a_socket, &dataBufLen)) != NULL)
					{
						#ifndef NOSERVERDEBUG
							printf("[%d] put(): about to write to file '%s'\n",controlData->sesion_id, dstPath);
						#endif
						struct timeval temps_inicial, temps_final;
						gettimeofday(&temps_inicial,NULL);
						tempStatus = service_writeFile(dstPath, dataBuf, dataBufLen);
						gettimeofday(&temps_final,NULL);
						float incMb = (float)dataBufLen/(1024*1024);
						double incTemps = (double) (temps_final.tv_sec - temps_inicial.tv_sec) 
						                            + ((temps_final.tv_usec - temps_inicial.tv_usec)/1000000.0);
						incrementarTransferencia(&controlData->stats.mbPut, incMb, &controlData->stats.tempsPut, 
						                            incTemps, &controlData->stats.numPut);
						controlData->stats.comandes += 1;


		                pthread_mutex_lock(&mutexServerStats);
		                gettimeofday(&serverStats.temps_final, NULL);
		                incrementarTransferencia(&serverStats.mbPut, incMb, &serverStats.tempsPut, 
						                            incTemps, &serverStats.numPut);
						incrementarComandes(&serverStats);
		                pthread_mutex_unlock(&mutexServerStats);
		                
						free(dataBuf);
						#ifndef NOSERVERDEBUG
							printf("[%d] put(): file writing %s.\n", controlData->sesion_id, tempStatus ? "OK" : "FAILED");
						#endif
						
						// send secondary ack: file was written OK
						if(tempStatus)
						{
							return service_sendStatus(a_socket, true);
						}
					}
					#ifndef NOSERVERDEBUG
					else
						printf("[%d] put(): getting of remote file failed.\n",controlData->sesion_id);
					#endif
				}
				#ifndef NOSERVERDEBUG
				else
					printf("[%d] put(): remote host didn't get status ACK.\n",controlData->sesion_id);
				#endif
			}
			#ifndef NOSERVERDEBUG
			else
				printf("[%d] put(): don't have write permissions.\n",controlData->sesion_id);
			#endif
		}
		#ifndef NOSERVERDEBUG
		else
			printf("[%d] put(): absolute path determining failed.\n",controlData->sesion_id);
		#endif
	}
	
	return false;
}

//if ctrl+c close server
void controlador(int numSignal)
{	
	printf("\nFinalitzant tasques...\n");
	fatality = 1;
	if(pthread_barrier_init(&end, NULL, threadsInUse))
	{
	    perror(ANSI_COLOR_RED "No se pude crear la barrera\n" ANSI_COLOR_RESET);
	}
	
    while(queue_finish.size != 0)
    {
        pthread_t tid = delQueue(&queue_finish);       ///XXX int no suficient per emmagatzemar TID
        if(pthread_join(tid, NULL) == -1)
        {
            perror("Error al finalitzar la petició del client\n");
        }else{
            threadsInUse--;
            sem_post(&threadsFree);
        }
    }
    pthread_join(destroyerTid, NULL);
    pthread_barrier_destroy(&end);
    
    pthread_mutex_destroy(&mutexQueue);
    pthread_mutex_destroy(&mutexServerStats);
    pthread_mutex_destroy(&mutexStats);
    
    pthread_cond_destroy(&finishSincro);
    
    destroyQueue(&queue_finish);
    
    pthread_mutex_destroy(&controlC);
    
    sem_destroy(&threadsFree);
    
	printf("\nRECUPERANT ESTADISTIQUES...\n");
	gettimeofday(&serverStats.temps_final, NULL);
	printStatsGlobals(&serverStats);
	close(serverSocket);
	exit(0);
}
