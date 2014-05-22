#include <math.h>  /* tan */
#include "Nave.h"
#include "Tiro.h"
#include "Cenario.h"

static void atualizaDirecao(double *ang);
static void direcionaProjetil(Projetil *bullet);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaNave(int zIni, int nVidas)
{
    /* Coordenadas iniciais */
    nave.base.x = 0.0;
    nave.base.y = Y_MAX/2;
    nave.base.z = zIni;

    /* Começa apontando para o centro */
    nave.angX = 0.0;
    nave.angY = 0.0;

    nave.vidas         = nVidas;
    nave.vz            = NAVE_VEL;
    nave.base.hp       = NAVE_HPMAX;
    nave.base.cooldown = NAVE_COOL;
    nave.base.espera   = nave.base.cooldown;
    nave.base.raio     = NAVE_RAIO;
    nave.base.altura   = NAVE_ALTURA;
}

/*------------------------------------------------------------------*/

void moveNave()
{
    nave.vx = nave.vz * tan(nave.angX);
    nave.vy = nave.vz * tan(nave.angY);

    nave.base.x += nave.vx;
    nave.base.y += nave.vy;
    nave.base.z += nave.vz;

    /* Nave tende a voltar ao centro */
    atualizaDirecao(&(nave.angX));
    atualizaDirecao(&(nave.angY));
}

/*------------------------------------------------------------------*/

void naveDispara()
{
    Projetil bullet;
    double k, r;
    double modulo = norma(nave.vx, nave.vy, nave.vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    k = BALA_VEL/modulo;
    bullet.vx = k * nave.vx;
    bullet.vy = k * nave.vy;
    bullet.vz = k * nave.vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    r = (nave.base.raio + BALA_RAIO)/modulo;
    bullet.x = nave.x + (r * nave.vx);
    bullet.y = nave.y + (r * nave.vy);
    bullet.z = nave.z + (r * nave.vz);

    bullet.dano = BALA_DANO;
    
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    nave.base.espera = nave.base.cooldown;
}

/*------------------------------------------------------------------*
 *
 *  Recebe um ponteiro para um ângulo de inclinação da nave e diminui
 *  seu valor em módulo. Caso chegue a 0°, direção é mantida.
 *
 */
static void atualizaDirecao(double *ang)
{
    if (*ang < 0.0) {
        *ang += ANG_AUTO;
        if (*ang > 0.0) *ang = 0.0;
    }
    else if (*ang > 0.0) {
        *ang -= ANG_AUTO;
        if (*ang < 0.0) *ang = 0.0;
    }
}
