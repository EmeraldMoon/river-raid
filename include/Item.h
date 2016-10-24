/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação de itens que ajudam o jogador (a.k.a. "powerups").  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>

#include "Base.h"
#include "Lista.h"
#include "Nave.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Tempo de espera até criar um item */
#define TEMPO_ITEM 240

/* Atributos padrão */
#define ITEM_RAIO 20

/* Tipos de item disponíveis no jogo */
// typedef enum {HP, VIDA, ESCUDO} ItemTipo;

/*
 *  Representa um item coletável.
 */
typedef struct item Item;
struct item
{
	Corpo corpo;

	/* Indica tipo do item */
    enum {HP, VIDA, ESCUDO} tipo;
};

extern Lista *items;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Insere um item na respectiva lista.
 */
void criaItem(Item *item);

/*
 *  Gera um item ao fundo do cenário, com altura, posição
 *  horizontal e outros atributos aleatórios.
 */
void geraItem();

/*
 *  Ativa o efeito do item na nave.
 */
void ativaItem(Item *item, Nave *nave);

/*
 *  Verifica se o item passou pela nave e saiu da tela.
 */
bool itemSaiu(Item *item);

/*
 *  Recebe um item e o desenha na tela.
 */
void desenhaItem(Item *item);
