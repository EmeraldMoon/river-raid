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
class Item : public Corpo
{
private:
    /* Indica tipo do item
       - HP aumenta barra de energia da nave
       - VIDA aumenta número de vidas
       - ESCUDO gera escudo provisória que toma parte do dano.
    */
    TipoItem tipo;

public:
    /*
     *  Gera um item na distância z, de posição horizontal e
     *  tipo aleatórios. Cada tipo de item possui uma
     *  probabilidade diferente de aparecer.
     */
    Item(double z);

    /*
     *  Ativa o efeito do item na nave.
     */
    void ativa();

    /*
     *  Recebe um item e o desenha na tela.
     */
    void desenha();
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Devolve ponteiro para lista de itens.
 */
std::vector<Item> *getListaItens();
