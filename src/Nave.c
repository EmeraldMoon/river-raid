#include <math.h>  /* tan*/
#include "Nave.h"
#include "Tiro.h"
#include "Cenario.h"

static void atualizaDirecao(double *ang);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaNave(int z, int nVidas)
{
    /* Coordenadas iniciais */
    nave.base.x = 0.0;
    nave.base.y = Y_MAX/2;
    nave.base.z = z;

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

    nave.invencibilidade = TEMPO_INVENCIVEL;
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

    bullet.dano = BALA_DANO;
    bullet.raio = BALA_RAIO;
    bullet.amigo = true;

    /* Componentes da velocidade da bala são proporcionais à nave */
    k = BALA_VEL/modulo;
    bullet.vx = k * nave.vx;
    bullet.vy = k * nave.vy;
    bullet.vz = k * nave.vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    r = (nave.base.raio + bullet.raio)/modulo;
    bullet.x = nave.base.x + (r * nave.vx);
    bullet.y = nave.base.y + (r * nave.vy);
    bullet.z = nave.base.z + (r * nave.vz);
    
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    nave.base.espera = nave.base.cooldown;
}

/*------------------------------------------------------------------*/

void danificaNave(int dano)
{
    nave.base.hp -= dano;
    nave.invencibilidade = TEMPO_INVENCIVEL;

    /* Verifica se nave perdeu vida */
    if (nave.base.hp <= 0) {
        nave.base.hp = 0;
        (nave.vidas)--;        
        if (nave.vidas > 0) criaNave(nave.base.z, nave.vidas);
    }
}

/*------------------------------------------------------------------*/

bool naveColidiu(Inimigo *foe)
{
    int dx = nave.base.x - foe->base.x;
    int dy = nave.base.y - foe->base.y;
    int dz = nave.base.z - foe->base.z;
    int somaRaios = nave.base.raio + foe->base.raio;

    /* Evita cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    return (norma(dx, 0, dz) < somaRaios)
        && (dy < (nave.base.altura + foe->base.altura)/2);
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
