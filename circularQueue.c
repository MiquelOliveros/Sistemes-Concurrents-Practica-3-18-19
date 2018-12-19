#include "circularQueue.h"

int addQueue(circularQueue *queue, double element)
{
    if(queue->size == queue->maxSize)
    {
        printf("NO he pasat CONDICIO:  %lf\n", element);
        return 0;
    }
    else{
        queue->tail = (queue->tail + 1) % queue->maxSize;
        queue->array[queue->tail] = element;
        queue->size++;
        return 1;
    }
}

double delQueue(circularQueue *queue)
{
    if(queue->size == 0)
    {
        return 0;
    }
    else{
        double elem = queue->array[(queue->heap)];
        queue->heap = (queue->heap + 1) % queue->maxSize;
        queue->size--;
        return elem;
    }
}

void initDefQueue(circularQueue *queue, int maxSize)
{
    queue->maxSize = maxSize;
    queue->size = 0;
    queue->heap = 0;
    queue->tail = -1;
    queue->array = (double *) malloc(sizeof(double) * queue->maxSize);
}

