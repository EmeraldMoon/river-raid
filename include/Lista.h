/*------------------------------------------------------------------*
 |                                                                  |
 |  Estruturas e funções para trabalhar com listas ligadas.         |
 |  As células podem aceitar qualquer tipo de dado como item.       |
 |  Cuida de toda a alocação e liberação de memória.                |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>  /* size_t, malloc, free, exit */

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/*
 *  Estrutura genérica de célula.
 */
typedef struct celula Celula;
struct celula
{
    void   *item;  /* ponteiro para tipo genérico */
    Celula *prox;
};

/* Para ser usado externamente */
typedef Celula Lista;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa uma lista (com cabeça).
 *  Devolve um ponteiro para a mesma.
 */
Lista * criaLista();

/*
 *  Adiciona um novo item no começo da lista.
 *  A função recebe um ponteiro para tipo genérico e o tamanho
 *  em bytes do mesmo, alocando a memória correspondente para uso.
 */
void insere(Celula *ini, void *item, size_t nbytes);

/*
 *  Recebe uma célula da lista e remove a seguinte, 
 *  liberando a memória do item e da célula em si.
 */
void exclui(Celula *cel);

/*
 *  Libera toda a memória alocada para a lista.
 */
void liberaLista(Celula *ini);

#endif
