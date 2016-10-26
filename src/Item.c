#include <math.h>  /* sqrt */

#include "Item.h"
#include "Random.h"
#include "Nave.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Textura.h"
#include "Cores.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de itens no cenário */
static Lista *itens;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaItens()
{
    itens = criaLista();
}

/*------------------------------------------------------------------*/

void geraItem(double z)
{
    /* Aloca espaço para o item */
    Item *item = mallocSafe(sizeof *item);

    /* Corpo cilídrico */
    item->corpo.raio   = ITEM_RAIO;
    item->corpo.altura = 2 * item->corpo.raio;
    posicionaCorpo(&item->corpo, z);

    /* Escolhe o tipo aleatoriamente */
    int sorte = uniforme(0, 100);
    if      (sorte  < 60)               return;
    else if (60 <= sorte && sorte < 85) item->tipo = HP;
    else if (86 <= sorte && sorte < 96) item->tipo = ESCUDO;
    else if (96 <= sorte)               item->tipo = VIDA;

    listaInsere(itens, item);
}

/*------------------------------------------------------------------*/

void ativaItem(Item *item, Nave *nave)
{
    switch (item->tipo) {
        case HP:
            nave->atribs.hp += nave->atribs.hp / 6;
            if (nave->atribs.hp > NAVE_HPMAX) {
                nave->atribs.hp = NAVE_HPMAX;
            }
            break;
        case VIDA:
            ++nave->vidas;
            break;
        case ESCUDO:
            nave->escudo = 2 * nave->atribs.hp;
            break;
    }
}

/*------------------------------------------------------------------*/

void desenhaItem(Item *item)
{
    static double rotacao = 0;
    rotacao += PI/6;

    glPushMatrix();
    glTranslated(item->corpo.x, item->corpo.y, item->corpo.z);
    glRotated(rotacao, 1.0, 1.0, 1.0);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glBindTexture(GL_TEXTURE_2D, itemTextura);

    switch(item->tipo) {
        case HP:
            getColorAlpha(LIGHT_GREEN, 230);
            glutSolidCube(ITEM_RAIO);
            break;
        case VIDA:
            getColorAlpha(GOLD, 130);
            glutSolidCube(ITEM_RAIO);
            break;
        case ESCUDO:
            getColorAlpha(DARK_BLUE, 130);
            glutWireSphere(ITEM_RAIO, SLICES, STACKS);
            break;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Lista *getListaItens()
{
    return itens;
}
