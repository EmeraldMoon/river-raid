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
    nave.base.x = 0;
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

/*------------------------------------------------------------*/

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

/*------------------------------------------------------------*/

void naveDispara()
{
    Projetil bullet;

    bullet.x = nave.base.x;
    bullet.y = nave.base.y;
    bullet.z = nave.base.z + nave.base.raio + BALA_RAIO;
    bullet.dano = BALA_DANO;

    direcionaProjetil(&bullet);
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    nave.base.espera = nave.base.cooldown;
}

/*------------------------------------------------------------*
 *
 *  Recebe um ponteiro para um ângulo de inclinação da nave e
 *  dimunui seu valor em módulo. Caso chegue a 0°, direção é
 *  mantida.
 *
 */
static void atualizaDirecao(double *ang)
{
    if (*ang < 0) {
        *ang += ANG_AUTO;
        if (*ang > 0) *ang = 0;
    }
    else if (*ang > 0) {
        *ang -= ANG_AUTO;
        if (*ang < 0) *ang = 0;
    }
}

/*------------------------------------------------------------*
 *
 *  Posiciona a velocidade do projétil na mesma direção e 
 *  sentido da da nave, proporcional em módulo.
 *
 */
static void direcionaProjetil(Projetil *bullet)
{
    double modulo = sqrt(sq(nave.vx) + sq(nave.vy) + sq(nave.vz));
    double k = BALA_VEL/modulo;

    bullet->vx = k * nave.vx;
    bullet->vy = k * nave.vy;
    bullet->vz = k * nave.vz;
}
