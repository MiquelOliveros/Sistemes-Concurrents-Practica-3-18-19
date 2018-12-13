/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#ifndef SERVER_H
#define SERVER_H

#include "siftp.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "service.h"



	/* constants */
	
		#define SERVER_SOCKET_BACKLOG	5
		#define SERVER_PASSWORD	"scp18"

//		#define CONCURRENT 1


		/* Funcions */

		/**
		 *Imprimeix per pantalla les stats del client/servidor
		 * @param stats Estructura amb les dades necessaries per fer els calculs
		**/
		void printStats(TStatsprog* stats);
		
		/**
		 *Acumula les dades dels diferents clients fent un comput general
		 * @param serv_stats son les estadistiques acumulades del servidor
		 * @param thread_stats son les estadistiques del client
		 * @note retorna 0 si ho ha pogut fer i 1 si algun error
		**/
		void join_Stats(TStatsprog* serv_stats, TStatsprog* thread_stats);
		/*
		 *
		**/
		void start_Stats(TStatsprog* stats);

	/* services */
	
		/**
		 * Establishes a network service on the specified port.
		 * @param	ap_socket	Storage for socket descriptor.
		 * @param	a_port	Port number in decimal.
		 */
		Boolean service_create(int *ap_socket, const int a_port);

		void hajimemasho(TControlTid *controlData);
		
		void shine_threads(TControlTid *controlData);
		
#endif