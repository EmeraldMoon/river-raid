#include <math.h>  /* sqrt */
#include "Defesa.h"
#include "Nave.h"
#include "Tiro.h"

static void miraProjetil(Projetil *bullet);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaInimigo(Inimigo foe)
{
    insere(inimigos, &foe, sizeof foe);
}

/*------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe)
{
    Projetil bullet;

    bullet.x = foe->base.x;
    bullet.y = foe->base.y;
    bullet.z = foe->base.z - (foe->base.raio + BALA_RAIO);
    bullet.dano = BALA_DANO;

    miraProjetil(&bullet);
    aplicaPrecisao(&bullet, foe->precisao);
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    foe->base.espera = foe->base.cooldown;
}

/*------------------------------------------------------------*/

bool inimigoSaiu(Inimigo *foe)
{
    return (foe->base.z < nave.base.z);
}

/*------------------------------------------------------------*
 *
 *  Mira o projéti no "bico" da nave, criando um vetor
 *  velocidade na referida direção e com módulo constante.
 *
 */
static void miraProjetil(Projetil *bullet)
{
    int dx = nave.base.x - bullet->x;
    int dy = nave.base.y - bullet->y;
    int dz = (nave.base.z + nave.base.raio) - bullet->z;

    /* Fator 'k' é a razão entre velocidade e distância */
    double d = sqrt(sq(dx) + sq(dy) + sq(dz));
    double k = BALA_VEL/d;

    bullet->vx = k * dx;
    bullet->vy = k * dy;
    bullet->vz = k * dz;
}
