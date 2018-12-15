#include <stdio.h>
#include <stdlib.h>

typedef struct{
    /*Afegint una variable size, podem controlar-ho bé
     *xro crec que hi ha d'aber un problema al condicional
     *de la linea 14*/
    int maxSize;
    int heap;
    int tail;
    int * array;
}circularQueue;

int addQueue(circularQueue *queue, int element){
    if(queue->heap == (queue->tail + 1) % queue->maxSize)
    {
        printf("NO %d\n", element);
        return 0;
    }
    else{
        queue->array[queue->tail] = element;
        queue->tail = (queue->tail + 1) % queue->maxSize;
        printf("%d\n", element);
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
    
    int maxThreads = 5;

    circularQueue queue;
    queue.maxSize = maxThreads;
    queue.heap = 0;
    queue.tail = 0;
    queue.array = (int *) malloc(sizeof(maxThreads));
    int res = 0;
    res = addQueue(&queue, 1);
    res = addQueue(&queue, 2);
    res = addQueue(&queue, 3);
    res = addQueue(&queue, 4);
    res = addQueue(&queue, 5);
    printf("Abans afegir cua %d\n", res);
    res = addQueue(&queue, 6);
    printf("Abans afegir cua %d\n", res);
    int element = queue.array[queue.heap];
    printf("Despres afegir cua %d\n", element);
    return 0;
}
