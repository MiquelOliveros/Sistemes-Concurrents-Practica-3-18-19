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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct{
			struct timeval temps_inicial;
			struct timeval temps_final;
			int comandes;
			int numPut;
			int numGet;
			double tempsPut;
			double tempsGet;
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
		
		/**
		 *Incrementa les comandes executades i comprova % 5
		 *@param stats Estructura que conté les estadístiques finals de la Sesio
		**/
		void incrementarComandes(TStatsprog* stats);
		
		/**
		 *Incrementa les estadístiques particulars del put i get
		 *@param mbTrans historial Mb trans
		 *@param incMb Mb transferits
		 *@param tempsTrans historial temps ocupat fent trans
		 *@param incTemps temps tardat en fer la trans
		 *@param comandesTrans comandes realitzades
		**/
		void incrementarTransferencia(float* mbTrans, float incMb, double* tempsTrans, double incTemps, int* comandesTrans);
		

		
