#include "circularQueue.h"

int addQueue(circularQueue *queue, int element)
{
    if(queue->size == queue->maxSize)
    {
        printf("NO he pasat CONDICIO:  %d\n", element);
        return 0;
    }
    else{
        queue->tail = (queue->tail + 1) % queue->maxSize;
        queue->array[queue->tail] = element;
        queue->size++;
        return 1;
    }
}

int delQueue(circularQueue *queue)
{
    if(queue->size == 0)
    {
        return 0;
    }
    else{
        int elem = queue->array[(queue->heap)];
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
    queue->array = (int *) malloc(sizeof(queue->maxSize));
}

