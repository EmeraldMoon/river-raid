#include <math.h>  /* tan */
#include "Cenario.h"
#include "Nave.h"
#include "Base.h"

static void atualizaDirecao(double ang);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaNave(int z)
{
    /* Coordenadas iniciais */
    nave.x = 0;
    nave.y = Y_MAX/2;
    nave.z = 0;

    /* Começa apontando para o centro */
    nave.angX = 0.0;
    nave.angY = 0.0;

    nave.vel      = NAVE_VEL;
    nave.hp       = NAVE_HPMAX;
    nave.cooldown = NAVE_COOL;
    nave.espera   = nave.cooldown;
}

/*------------------------------------------------------------*/

void moveNave()
{
    vx = nave.vel * tan(nave.angX);
    vy = nave.vel * tan(nave.angY);

    nave.x += nave.vx;
    nave.y += nave.vy;
    nave.z += nave.vel;

    /* Nave tende a voltar ao centro */
    atualizaDirecao(&(nave.angX));
    atualizaDirecao(&(nave.angY));
}

/*------------------------------------------------------------*/

void naveDispara()
{
    Projetil bullet;

    bullet.x = nave.x;
    bullet.y = nave.y;
    bullet.z = nave.z + NAVE_RAIO;
    bullet.dano = BALA_DANO;

    direcionaProjetil(&bullet);
    criaProjetil(bullet);

    nave.espera = nave.cooldown;
}

/*------------------------------------------------------------*
 *
 *  Recebe um ponteiro para um ângulo de inclinação da nave.
 *  Dimunui seu valor em módulo. Caso chegue a 0°, direção é mantida.
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
        if (*ang > 0) *ang = 0;
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
    bullet->vz = k * nave.vel;
}
