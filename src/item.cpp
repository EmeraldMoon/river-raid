#include <cmath>  /* sqrt */
#include <vector>

#include "item.hpp"
#include "random.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------*
 |   I T E M   |
 *-------------*----------------------------------------------------*/

Item::Item(double z) : Corpo(z)
{
    raio   = 20.0;
    altura = 2 * raio;

    /* Escolhe o tipo aleatoriamente */
    int sorte = uniforme(0, 100);
    if      (sorte  < 50)                return;
    else if (50 <= sorte and sorte < 80) tipo = TipoItem::HP;
    else if (80 <= sorte and sorte < 95) tipo = TipoItem::ESCUDO;
    else if (95 <= sorte)                tipo = TipoItem::VIDA;
}

/*------------------------------------------------------------------*/

void Item::desenha()
{
    static double rotacao = 0;
    rotacao += M_PI/6;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Posiciona e rotaciona item */
    glTranslated(x, y, z);
    glRotated(rotacao, 1.0, 1.0, 1.0);

    switch(tipo) {
    case TipoItem::HP:
        setColorAlpha(LIGHT_GREEN, 230);
        glutSolidCube(raio);
        break;
    case TipoItem::VIDA:
        setColorAlpha(GOLD, 130);
        glutSolidCube(raio);
        break;
    case TipoItem::ESCUDO:
        setColorAlpha(DARK_BLUE, 130);
        glutWireSphere(raio, SLICES, STACKS);
        break;
    }
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

TipoItem Item::getTipo() { return tipo; }
