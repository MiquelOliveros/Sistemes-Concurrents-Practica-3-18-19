/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#include <"stats.h">

void start_Stats(TStatsprog* stats)
{
	stats->temps_inicial = clock(); /*Eliminar un cop acabades les proves noves*/
	stats->comandes = 0;
	stats->tempsPut = 0;
	stats->numPut = 0;
	stats->tempsGet = 0;
	stats->numGet = 0;
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
}

//TODO modificar per calcular en temps real (tempsPut, tempsGet)
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

	mbPutsec = stats->mbPut/(temps*1024);
	printf("Velocitat de pujada:\t\t\t%f MB/s\n",mbPutsec);

	printf("Fitxers descarregats:\t\t\t%d\n", stats->numGet);

	mbGetsec = stats->mbGet/(temps*1024);
	printf("Velocitat de baixada:\t\t\t%f MB/s\n",mbGetsec);
}

void printStatsGlobals(TStatsprog* stats)
{
    printf("%sSYSTEM NOTICE: Updated server Stats\n", KGRN);
    printf("%s###########################################", KGRN);
    printStats(stats);
    printf("%s###########################################", KGRN);
}

void printSesionStats(TStatsprog* stats)
{
    printf("%SSYSTEM NOTICE: Client stats Report\n", KYEL);
    printf("%s###########################################", KYEL);
    printStats(stats);
    printf("%s###########################################", KYEL);
}
