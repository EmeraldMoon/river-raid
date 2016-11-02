/*------------------------------------------------------------------*
 |                                                                  |
 |  Estruturas e funções para trabalhar com listas ligadas.         |
 |  As células podem aceitar qualquer tipo de dado como item.       |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/*
 *  Estrutura genérica de célula.
 */
typedef struct celula Celula;
struct celula
{
    void *item;  /* ponteiro para tipo genérico */
    Celula *prox;
};

/* Para ser usado externamente */
typedef Celula Lista;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Inicializa uma lista (com cabeça).
 *  Devolve um ponteiro para a mesma.
 */
Lista *criaLista();

/*
 *  Recebe um ponteiro genérico previamente alocado.
 *  Adiciona o novo item no começo da lista.
 */
void listaInsere(Lista *lista, void *item);

/*
 *  Recebe uma célula da lista e remove a seguinte, 
 *  liberando a memória do item e da célula em si.
 */
void listaRemoveProx(Celula *cel);

/*
 *  Libera toda a memória alocada para a lista.
 */
void liberaLista(Lista *lista);
