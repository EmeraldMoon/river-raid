#include "Nave.h"
#include "Tiro.h"
#include "Defesa.h"

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

    bullet.x = foe->x;
    bullet.y = foe->y;
    bullet.z = foe->z - FOE_RAIO;
    bullet.dano = BALA_DANO;

    miraProjetil(&bullet);
    aplicaPrecisao(&bullet, foe->precisao);
    criaProjetil(bullet);

    foe->espera = foe->cooldown;
}

/*------------------------------------------------------------*/

bool inimigoSaiu(Inimigo *foe)
{
    return (foe->z < nave.z);
}

/*------------------------------------------------------------*
 *
 *  Mira o projéti no "bico" da nave, criando um vetor
 *  velocidade na referida direção e com módulo constante.
 *
 */
static void miraProjetil(Projetil *bullet)
{
    int dx = nave.x - bullet->x;
    int dy = nave.y - bullet->y;
    int dz = (nave.z + NAVE_RAIO) - bullet->z;

    /* Fator 'k' é a razão entre velocidade e distância */
    double d = sqrt(sq(dx) + sq(dy) + sq(dz));
    double k = BALA_VEL/d;

    bullet->vx = k * dx;
    bullet->vy = k * dy;
    bullet->vz = k * dz;
}
