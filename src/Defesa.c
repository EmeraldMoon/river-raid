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

/*------------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe)
{
    Projetil bullet;
    int dx, dy, dz;
    double d, k, r;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    dx = foe->base.x - nave.base.x;
    dy = foe->base.y - nave.base.y;
    dz = foe->base.z - (nave.base.z + nave.base.raio);

    d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    k = BALA_VEL/d;
    bullet.vx = k * dx;
    bullet.vy = k * dy;
    bullet.vz = k * dz;

    r = (foe->base.raio + BALA_RAIO)/d;
    bullet.x = foe->base.x + (r * dx);
    bullet.y = foe->base.y + (r * dy);
    bullet.z = foe->base.z + (r * dz);

    bullet.dano = BALA_DANO;
    
    aplicaPrecisao(&bullet, foe->precisao);
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    foe->base.espera = foe->base.cooldown;
}

/*------------------------------------------------------------------*/

bool inimigoSaiu(Inimigo *foe)
{
    return (foe->base.z < nave.base.z);
}
