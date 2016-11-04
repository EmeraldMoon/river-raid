#include <cmath>  /* sqrt */
#include <vector>

#include "item.hpp"
#include "random.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de itens no cenário */
std::vector<Item> itens;

/*-------------*
 |   I T E M   |
 *-------------*----------------------------------------------------*/

Item::Item(double z) : Corpo(z)
{
    /* Escolhe o tipo aleatoriamente */
    TipoItem tipo;
    int sorte = uniforme(0, 100);
    if      (sorte  < 60)                       return;
    else if (60 <= sorte and sorte < 85) tipo =     HP;
    else if (86 <= sorte and sorte < 96) tipo = ESCUDO;
    else if (96 <= sorte)                tipo =   VIDA;

    raio   = ITEM_RAIO;
    altura = 2 * raio;
    tipo   = tipo;
}

/*------------------------------------------------------------------*/

void Item::desenha()
{
    static double rotacao = 0;
    rotacao += PI/6;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Posiciona e rotaciona item */
    glTranslated(x, y, z);
    glRotated(rotacao, 1.0, 1.0, 1.0);

    switch(tipo) {
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

TipoItem Item::getTipo() { return tipo; }

/*------------------------------------------------------------------*/

std::vector<Item> *getListaItens()
{
    return &itens;
}
