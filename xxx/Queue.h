#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Defesa.h"

#ifndef QUEUE_H
#define QUEUE_H

typedef struct node Node;
struct node
{
    void *item; /* apontador para tipo genérico */
    Node *next;
};

typedef struct queue Queue;
struct queue
{
    Node *first;
    Node *last;
};

Queue * queueInit();
bool queueEmpty(Queue *q);
void   enqueue(Queue *q, void *item, size_t nbytes);
void * dequeue(Queue *q);
void queueFree(Queue *q);

static void * mallocSafe(size_t nbytes);

#endif
