/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#include "stats.h"

void start_Stats(TStatsprog* stats)
{
	stats->temps_inicial = clock(); /*Eliminar un cop acabades les proves noves*/
	stats->comandes = 0;
	stats->numPut = 0;
	stats->numGet = 0;
	stats->tempsPut = 0;
	stats->tempsGet = 0;
	stats->mbPut = 0;
	stats->mbGet = 0;	
}

//XXX NO UTILITZAR EN LA NOVA VERSIO
void join_Stats(TStatsprog* serv_stats, TStatsprog* thread_stats) 
{
	serv_stats->comandes = serv_stats->comandes + thread_stats->comandes;
	serv_stats->numPut = serv_stats->numPut + thread_stats->numPut;
	serv_stats->numGet = serv_stats->numGet + thread_stats->numGet;
	serv_stats->mbPut = serv_stats->mbPut + thread_stats->mbPut;
	serv_stats->mbGet = serv_stats->mbGet + thread_stats->mbGet;
	serv_stats->tempsPut = serv_stats->tempsPut + thread_stats->tempsPut;
	serv_stats->tempsGet = serv_stats->tempsGet + thread_stats->tempsGet;
}

//TODO modificar per calcular en temps real (tempsPut, tempsGet) clock DOES NOT WORK
void printStats(TStatsprog* stats)
{
	float min;
	float comandesMin, mbPutsec, mbGetsec;
	double temps =(double)((stats->temps_final - stats->temps_inicial) / 1000000.0); //(temps en s) 
	printf("Temps connexio:\t\t\t%lf s\n", temps);
	printf("Comandes executades:\t\t\t%d\n", stats->comandes);

	min = temps / 60;
	comandesMin = (float) stats->comandes / min;
	printf("Comandes/min executades:\t\t%f\n", comandesMin);

	printf("Fitxers pujats:\t\t\t\t%d\n", stats->numPut);
    
    printf("Temps de pujada:\t\t\t%lf s\n",stats->tempsPut);
	mbPutsec = (float) stats->mbPut/(float)(stats->tempsPut*1024);
	printf("Velocitat de pujada:\t\t\t%f MB/s\n",mbPutsec);

	printf("Fitxers descarregats:\t\t\t%d\n", stats->numGet);

    printf("Temps de baixada:\t\t\t%lf s\n",stats->tempsGet);
	mbGetsec = (float) stats->mbGet/((float)stats->tempsGet*1024);
	printf("Velocitat de baixada:\t\t\t%f MB/s\n",mbGetsec);
}

void printStatsGlobals(TStatsprog* stats)
{
    printf(ANSI_COLOR_GREEN "SYSTEM NOTICE: Updated server Stats" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "###########################################" ANSI_COLOR_RESET "\n");
    printStats(stats);
    printf(ANSI_COLOR_GREEN "###########################################" ANSI_COLOR_RESET "\n");
}

void printSesionStats(TStatsprog* stats)
{
    printf(ANSI_COLOR_YELLOW "SYSTEM NOTICE: Client stats Report" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_YELLOW "###########################################" ANSI_COLOR_RESET "\n");
    printStats(stats);
    printf(ANSI_COLOR_YELLOW "###########################################" ANSI_COLOR_RESET "\n");
}

void incrementarComandes(TStatsprog* stats)
{
    stats->comandes += 1;
    if(stats->comandes % 5 == 0){
        printSesionStats(stats);        //XXX posar estats servidor
    }
}

void incrementarTransferencia(float* mbTrans, float incMb, double* tempsTrans, double incTemps, int* comandesTrans)
{
    *comandesTrans +=1;
    *tempsTrans += incTemps;
    *mbTrans += incMb;
}
