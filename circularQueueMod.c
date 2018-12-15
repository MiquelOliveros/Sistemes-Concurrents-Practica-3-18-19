#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

typedef struct{
    /*Afegint una variable size, podem controlar-ho bé
     *xro crec que hi ha d'aber un problema al condicional
     *de la linea 14*/
    int maxSize;
    int heap;
    int tail;
    int * array;
    int size
}circularQueue;

int addQueue(circularQueue *queue, int element){
    if(queue->tail == queue->size-1 && queue->heap ==0 || queue->heap == (queue->tail + 1) % queue->maxSize)
    {
        printf("NO he pasat CONDICIO:  %d\n", element);
        return 0;
    }else{
        queue->tail = (queue->tail + 1) % queue->maxSize;
        queue->array[queue->tail] = element;
        queue->size++;
        return 1;
    }
}

int* delQueue(circularQueue *queue){
    if(queue->heap == (queue->tail))
    {
        return 0;
    }
    else{
        queue->heap = (queue->heap + 1) % queue->maxSize;
        return queue->array[(queue->heap)-1];
    }
}



int main(){

    circularQueue queue;
    queue.maxSize = SIZE;
    queue.size = 0;
    queue.heap = -1;
    queue.tail = -1;
    queue.array = (int *) malloc(sizeof(SIZE));
    int res = 0;
    for(int i=1; i < 7; i++){
        printf("Abans afegir cua %d\n", i);
        res = addQueue(&queue, i);
        int element = queue.array[queue.heap];
        printf("Despres afegir cua el HEAP es: %d\n", element);
        
    }
    for(int j=0; j< queue.maxSize; j++){
        printf("%d-->%d\t",j, queue.array[j]);
        printf("\n");
    }
    printf("HEAP = %d\n",queue.array[queue.heap]);
    printf("TAIL = %d\n",queue.array[queue.tail]);    
    return 0;
}
