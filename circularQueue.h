#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
    int maxSize;
    int heap;
    int tail;
    int size;
    pthread_t *array; 
}circularQueue;

		/* Funcions */
		
		
		/**
		 * Inicialitza l'estructura de dades que s'empra per emmagatzemar estadístiques
		 *@param queue punter a la cua circular que conté les dades
		 *@param element integer a inserir a la cua
		**/
        int addQueue(circularQueue *queue, pthread_t element);

		/**
		 * Inicialitza l'estructura de dades que s'empra per emmagatzemar estadístiques
		 *@param queue punter a  la cua circular que conté les dades
		**/
        double delQueue(circularQueue *queue);

		/**
		 * Inicialitza l'estructura de dades que s'empra per comunicar la finalitzacio de tids
		 *@param queue Estructura a inicialitzar
		 *@param maxSize Tamany maxim de la cua
		**/
        void initDefQueue(circularQueue *queue, int maxSize);
        
        /**
		 * Allibera l'estructura
		 *@param queue Estructura a destruir
		**/
        void destroyQueue(circularQueue* queue);
        



        /*
        for(int i = 0; i < queue->maxSize; i++)
        {
            printf("%d-->%d\n", i, queue->array[i]);
        }
        */        
/*
Proves realitzades per observar el funcionament
int main(){

    circularQueue queue;
    queue.maxSize = 5;
    queue.size = 0;
    queue.heap = 0;
    queue.tail = -1;
    queue.array = (int *) malloc(sizeof(queue.maxSize));
    int res = 0;
    for(int i=1; i < 7; i++){
        printf("Abans afegir cua %d\n", i);
        int element = queue.array[queue.heap];
        printf("Abans afegir cua el HEAP es: %d\n", element);
        element = queue.array[queue.tail];
        printf("Abans afegir cua el TAIL es: %d\n", element);
        if(i == 6){
            res = delQueue(&queue);
            printf("Retorn borrar de la cua %d\n", res);
            element = queue.array[queue.heap];
            printf("Despres borrar cua el HEAP es: %d\n", element);
            element = queue.array[queue.tail];
            printf("Desepres borrar cua el TAIL es: %d\n", element);
        }    
        res = addQueue(&queue, i);
        element = queue.array[queue.heap];
        printf("Despres afegir cua el HEAP es: %d\n", element);
        element = queue.array[queue.tail];
        printf("Despres afegir cua el TAIL es: %d\n", element);
        
    }
    for(int j=0; j< queue.maxSize; j++){
        printf("%d-->%d\t",j, queue.array[j]);
        printf("\n");
    }
    printf("HEAP = %d\n",queue.array[queue.heap]);
    printf("TAIL = %d\n",queue.array[queue.tail]);
    
    for(int i = queue.size; i >= 0; i--)
    {
        res = delQueue(&queue);
        printf("Retorn borrar de la cua %d\n", res);
        int element = queue.array[queue.heap];
        printf("Despres borrar cua el HEAP es: %d\n", element);
        element = queue.array[queue.tail];
        printf("Desepres borrar cua el TAIL es: %d\n", element);
    }     
    return 0;
}
*/
