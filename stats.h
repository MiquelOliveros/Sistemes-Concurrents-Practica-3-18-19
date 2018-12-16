/**
*################################################################
*#   Codi Font													#
*#	Practica 1 Sistemes Concurrents i Paralels					#
*#	Alumnes:													#
*#		Joan Palau Oncins		- 47692135W						#
*#		Miquel Oliveros Muelas	- 0024201						#
*################################################################ 
**/


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define KNRM  "\x1B[0m"     
#define KRED  "\x1B[31m"    
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef struct{
			clock_t temps_inicial;
			clock_t temps_final;
			int comandes;
			int tempsPut;
			int numPut;
			int tempsGet;
			int numGet;
			float mbPut;
			float mbGet;
}TStatsprog;

		/* Funcions */
		
		/**
		 * Inicialitza l'estructura de dades que s'empra per emmagatzemar estadístiques
		 *@param stats Estructura a inicialitzar
		**/
		void start_Stats(TStatsprog* stats);
		
		/**
		 * XXX En desús, no es pot emprar en el calcul en temps real
		 *Acumula les dades dels diferents clients fent un comput general
		 * @param serv_stats son les estadistiques acumulades del servidor
		 * @param thread_stats son les estadistiques del client
		 * @note retorna 0 si ho ha pogut fer i 1 si algun error
		**/
		void join_Stats(TStatsprog* serv_stats, TStatsprog* thread_stats);

		/**
		 *Imprimeix per pantalla i computa unes estadístiques donades
		 * @param stats Estructura amb les dades necessaries per fer els calculs
		**/
		void printStats(TStatsprog* stats);
		
		/**
		 *Imprimeix per pantalla les estadístiques del servidor
		 *@param stats Estructura que conté les estadístiques Globals
		**/
		void printStatsGlobals(TStatsprog* stats);
		
		/**
		 *Imprimeix per pantalla les estadístiques del client
		 *@param stats Estructura que conté les estadístiques finals de la Sesio
		**/
		void printSesionStats(TStatsprog* stats);
		

		
