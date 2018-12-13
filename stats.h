/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/

#include "siftp.h"
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

/* Estructures de dades */
typedef struct{
			int temps;
			int comandes;
			int numPut;
			int numGet;
			float mbPut;
			float mbGet;
}TStatsprog;

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
		Boolean join_Stats(TStatsprog* serv_stats, TStatsprog* thread_stats);
		/*
		 *
		**/
		Boolean start_Stats(TStatsprog* stats);