#include <math.h>  /* sqrt */

#include "Item.h"
#include "Random.h"
#include "Nave.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Textura.h"

static double rotacao = 0;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaItem(Item *item)
{
    listaInsere(items, item);
}

/*------------------------------------------------------------------*/

void geraItem(int z)
{
    Item *item = mallocSafe(sizeof *item);
    int sorte = uniformeInt(0, 100);

    if      (sorte  < 60)               return;
    else if (sorte >= 60 && sorte < 85) item->tipo = HP;
    else if (sorte >= 86 && sorte < 96) item->tipo = ESCUDO;
    else if (sorte >= 96)               item->tipo = VIDA;

    item->corpo.x = uniformeDouble(-X_MAX + ITEM_RAIO, X_MAX - ITEM_RAIO);
    item->corpo.y = uniformeInt(Y_MAX/8, Y_MAX/2);
    item->corpo.z = z;

    criaItem(item);
}

/*------------------------------------------------------------------*/

void ativaItem(Item *item, Nave *nave)
{
    switch(item->tipo) {
        case HP:
            nave->atribs.hp += NAVE_HPMAX/6;
            if (nave->atribs.hp > NAVE_HPMAX) nave->atribs.hp = NAVE_HPMAX;
            break;
        case VIDA:
            ++nave->vidas;
            break;
        case ESCUDO:
            nave->escudo = 2*NAVE_HPMAX;
            break;
    }
}

/*------------------------------------------------------------------*/

void desenhaItem(Item *item)
{
    rotacao += PI/6;

    glPushMatrix();
    glTranslated(item->corpo.x, item->corpo.y, item->corpo.z);
    glRotated(rotacao, 1.0, 1.0, 1.0);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glBindTexture(GL_TEXTURE_2D, itemTextura);

    switch(item->tipo) {
        case HP:
            glColorAlpha(LIGHT_GREEN, 230);
            glutSolidCube(ITEM_RAIO);
            break;
        case VIDA:
            glColorAlpha(GOLD, 130);
            glutSolidCube(ITEM_RAIO);
            break;
        case ESCUDO:
            glColorAlpha(DARK_BLUE, 130);
            glutWireSphere(ITEM_RAIO, SLICES, STACKS);
            break;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}
