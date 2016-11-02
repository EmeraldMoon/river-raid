/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação de itens que ajudam o jogador (a.k.a. "powerups").  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include "base.hpp"
#include "nave.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Tempo de espera até criar um item */
#define TEMPO_ITEM 240

/* Atributos padrão */
#define ITEM_RAIO 20

/*------------------------------------------------------------------*/

/*
 *  Enumerador para tipos de item.
 */
enum tipoItem
{
    HP, VIDA, ESCUDO
};
typedef enum tipoItem TipoItem;

/*
 *  Representa um item coletável.
 */
typedef struct item Item;
struct item
{
    /* Corpo esférico */
	Corpo corpo;

	/* Indica tipo do item
       - HP aumenta barra de energia da nave
       - VIDA aumenta número de vidas
       - ESCUDO gera escudo provisória que toma parte do dano.
    */
    TipoItem tipo;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Gera um item na distância z, de posição horizontal e
 *  tipo aleatórios. Cada tipo de item possui uma
 *  probabilidade diferente de aparecer.
 */
void geraItem(double z);

/*
 *  Ativa o efeito do item na nave.
 */
void ativaItem(Item *item, Nave *nave);

/*
 *  Recebe um item e o desenha na tela.
 */
void desenhaItem(Item *item);

/*
 *  Devolve ponteiro para lista de itens.
 */
std::vector<Item> *getListaItens();
