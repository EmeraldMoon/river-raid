/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação de itens que ajudam o jogador (a.k.a. "powerups").  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "base.hpp"

/*------------------------------------------------------------------*/

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
     *  Cria um item na distância z, de posição horizontal e
     *  tipo aleatórios. Cada tipo de item possui uma
     *  probabilidade diferente de aparecer.
     */
    Item(double z);

    /*
     *  Desenha o item na tela.
     */
    void desenha();

    /* Getters */
    TipoItem getTipo();
};

/*------------------------------------------------------------------*/

/*
 *  Devolve ponteiro para lista de itens.
 */
std::vector<Item> *getListaItens();
