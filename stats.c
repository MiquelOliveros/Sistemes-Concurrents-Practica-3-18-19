/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#include <stdio.h>



void printStats(TStatsprog* stats)
{
	int min, mbPutsec, mbGetsec;
	float comandesMin;

	printf("Temps connexio:\t\t%d\n", stats->temps);
	printf("Comandes executades:\t\t%d\n", stats->comandes);

	min = stats->temps/60;
	comandesMin = stats->comandes/min;
	printf("Comandes/min executades:\t\t%f\n", comandesMin);

	printf("Fitxers pujats:\t\t%d\n", stats->numPut);

	mbPutsec = stats->mbPut/(stats->temps*1024);
	printf("Velocitat de pujada:\t\t%d MB/s\n",mbPutsec);

	printf("Fitxers descarregats:\t\t%d\n", stats->numGet);

	mbGetsec = stats->mbGet/(stats->temps*1024);
	printf("Velocitat de baixada:\t\t%d MB/s\n",mbGetsec);
}

Boolean join_Stats(TStatsprog* serv_stats, TStatsprog* thread_stats)
{
	serv_stats->temps = serv_stats->temps + thread_stats->temps;
	serv_stats->comandes = serv_stats->comandes + thread_stats->comandes;
	serv_stats->numPut = serv_stats->numPut + thread_stats->numPut;
	serv_stats->numGet = serv_stats->numGet + thread_stats->numGet;
	serv_stats->mbPut = serv_stats->mbPut + thread_stats->mbPut;
	serv_stats->mbGet = serv_stats->mbGet + thread_stats->mbGet;

	return 0;
}

Boolean start_Stats(TStatsprog* stats)
{
	serv_stats->temps = 0;
	serv_stats->comandes = 0;
	serv_stats->numPut = 0;
	serv_stats->numGet = 0;
	serv_stats->mbPut = 0;
	serv_stats->mbGet = 0;	
}