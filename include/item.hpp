/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação de itens que ajudam o jogador (a.k.a. "powerups").  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "base.hpp"

/*------------------------------------------------------------------*/

/*
 *  Enumerador para tipos de item.
 */
enum class TipoItem
{
    HP, VIDA, ESCUDO
};

/*
 *  Representa um item coletável.
 */
class Item : public Corpo
{
private:
    /* Indica tipo do item
       - HP aumenta barra de energia da nave
       - VIDA aumenta número de vidas
       - ESCUDO gera escudo provisório que toma parte do dano.
    */
    TipoItem tipo;

public:
    static Lista<Item> lista;

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
