#include "Queue.h"

/*
 *  Inicializa a fila e devolve um ponteiro para a mesma.
 */

Queue * queueInit()
{
    Queue *q = mallocSafe(sizeof (Queue));
    q->first = q->last = NULL;
    return q;
}

/*
 *  Verifica se a fila está vazia.
 */

bool queueEmpty(Queue *q)
{
    return (q->first == NULL);
}

/*
 *  Adiciona um novo item ao final da fila.
 *  A função recebe um ponteiro para tipo genérico e o tamanho
 *  em bytes do mesmo, o qual deve ser obtido por 'sizeof' na 
 *  chamada da função. A memória correspondente é alocada para uso.
 */

void enqueue(Queue *q, void *item, size_t nbytes)
{
    Node *oldlast = q->last;
    q->last = mallocSafe(sizeof (Node));
    q->last->next = NULL;
    if (queueEmpty(q)) q->first = q->last;
    else          oldlast->next = q->last;

    /* Aloca espaço para o item e copia seu conteúdo */
    q->last->item = mallocSafe(nbytes);
    memcpy(q->last->item, item, nbytes);
}

/*
 *  Remove o primeiro item da fila e devolve um ponteiro
 *  para o mesmo. Nota: como a função necessita devolver um
 *  endereço, ela não realiza free() do mesmo. A liberação de
 *  memória deve ser feita logo em seguida à chamada desta.
 */

void * dequeue(Queue *q)
{
    Node *oldfirst = q->first;
    void *item = oldfirst->item;
    q->first = oldfirst->next;
    free(oldfirst); oldfirst = NULL;
    if (queueEmpty(q)) q->last = NULL;
    return item;
}

/*
 *  Libera toda a memória alocada para a fila.
 */

void queueFree(Queue *q)
{
    Node *p = q->first;
    while (p != NULL) {
        Node *aux = p;
        p = p->next;
        free(aux->item); aux->item = NULL;
        free(aux); aux = NULL;
    }
    free(q); q = NULL;
}

/*
 *  Versão segura de malloc().
 *  Caso não haja memória disponível, exibe uma mensagem de
 *  erro e encerra o programa.
 */

static void * mallocSafe(size_t nbytes)
{
   void *ptr = malloc(nbytes);
   if (ptr == NULL) {
      perror("ERRO");
      exit(EXIT_FAILURE);
   }
   return ptr;
}
