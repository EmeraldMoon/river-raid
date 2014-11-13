/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação de itens que ajudam o jogador (a.k.a. "powerups").  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include "Lista.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Tempo de espera até criar um item */
#define TEMPO_ITEM 240

/* Atributos padrão */
#define ITEM_RAIO 20

/* Tipos de item disponíveis no jogo */
typedef enum {HP, VIDA, ESCUDO} ItemTipo;

/*
 *  Representa um inimigo (elemento de defesa).
 */
typedef struct item Item;
struct item
{
    ItemTipo tipo; /* que tipo de item é */
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o início do cenário */
};

extern Lista *items;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Insere um item na respectiva lista.
 */
void criaItem(Item item);

/*
 *  Gera um item ao fundo do cenário, com altura, posição
 *  horizontal e outros atributos aleatórios.
 */
void geraItem();

/*
 *  Ativa o efeito do item.
 */
void ativaItem(Item *item);

/*
 *  Verifica se o item passou pela nave e saiu da tela.
 */
bool itemSaiu(Item *item);

/*
 *  Recebe um item e o desenha na tela.
 */
void desenhaItem(Item *item);
