#include <cmath>  /* sqrt */
#include <vector>

#include "item.hpp"
#include "random.hpp"
#include "nave.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de itens no cenário */
std::vector<Item> itens;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void geraItem(double z)
{
    /* Escolhe o tipo aleatoriamente */
    TipoItem tipo;
    int sorte = uniforme(0, 100);
    if      (sorte  < 60)               return;
    else if (60 <= sorte && sorte < 85) tipo =     HP;
    else if (86 <= sorte && sorte < 96) tipo = ESCUDO;
    else if (96 <= sorte)               tipo =   VIDA;

    Item item;
    geraCorpo(&item, z);
    item.raio   = ITEM_RAIO;
    item.altura = 2 * item.raio;
    item.tipo = tipo;

    itens.push_back(item);
}

/*------------------------------------------------------------------*/

void ativaItem(Item *item, Nave *nave)
{
    switch (item->tipo) {
        case HP:
            nave->hp += nave->hp / 6;
            if (nave->hp > NAVE_HPMAX) {
                nave->hp = NAVE_HPMAX;
            }
            break;
        case VIDA:
            ++nave->vidas;
            break;
        case ESCUDO:
            nave->escudo = 2 * nave->hp;
            break;
    }
}

/*------------------------------------------------------------------*/

void desenhaItem(Item *item)
{
    static double rotacao = 0;
    rotacao += PI/6;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Posiciona e rotaciona item */
    glTranslated(item->x, item->y, item->z);
    glRotated(rotacao, 1.0, 1.0, 1.0);

    switch(item->tipo) {
        case HP:
            setColorAlpha(LIGHT_GREEN, 230);
            glutSolidCube(ITEM_RAIO);
            break;
        case VIDA:
            setColorAlpha(GOLD, 130);
            glutSolidCube(ITEM_RAIO);
            break;
        case ESCUDO:
            setColorAlpha(DARK_BLUE, 130);
            glutWireSphere(ITEM_RAIO, SLICES, STACKS);
            break;
    }
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

std::vector<Item> *getListaItens()
{
    return &itens;
}
