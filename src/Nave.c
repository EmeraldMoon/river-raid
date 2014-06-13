#include <math.h>  /* tan, abs */
#include <GL/freeglut.h>
#include "Nave.h"
#include "Cenario.h"
#include "Grafico.h"

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
    nave.angHoriz = 0.0;
    nave.angVert  = 0.0;

    nave.vidas         = nVidas;
    nave.vz            = NAVE_VEL;
    nave.base.hp       = NAVE_HPMAX;
    nave.base.cooldown = NAVE_COOL;
    nave.base.espera   = 0;
    nave.base.raio     = NAVE_RAIO;
    nave.base.altura   = NAVE_ALTURA;

    nave.invencibilidade = INVENCIVEL_VIDA;
}

/*------------------------------------------------------------------*/

static void atualizaDirecao(double *ang);

void moveNave()
{
    const int RAIO   = nave.base.raio;
    const int ALTURA = nave.base.altura;

    nave.vx = nave.vz * tan(nave.angHoriz);
    nave.vy = nave.vz * tan(nave.angVert);

    nave.base.x += nave.vx;
    nave.base.y += nave.vy;
    nave.base.z += nave.vz;

    /* Impede que nave ultrapasse os limites do cenário */
    if      (nave.base.x >  X_MAX - RAIO)   nave.base.x =  X_MAX - RAIO;
    else if (nave.base.x < -X_MAX + RAIO)   nave.base.x = -X_MAX + RAIO;
    if      (nave.base.y >  Y_MAX - ALTURA) nave.base.y =  Y_MAX - ALTURA;
    else if (nave.base.y <      0 + ALTURA) nave.base.y =      0 + ALTURA;

    /* Direção tende a voltar ao centro */
    atualizaDirecao(&(nave.angHoriz));
    atualizaDirecao(&(nave.angVert));
}

/*
 *  Recebe um ponteiro para um ângulo de inclinação da nave e diminui
 *  seu valor em módulo. Caso chegue a 0°, direção é mantida.
 */
static void atualizaDirecao(double *ang)
{
    if (*ang > 0.0) {
        *ang -= ANG_AUTO;
        if (*ang < 0.0) *ang = 0.0;
    }
    else if (*ang < 0.0) {
        *ang += ANG_AUTO;
        if (*ang > 0.0) *ang = 0.0;
    }
}

/*------------------------------------------------------------------*/

void naveDispara()
{
    Projetil bullet;
    double modulo = norma(nave.vx, nave.vy, nave.vz);

    bullet.dano = BALA_DANO;
    bullet.raio = BALA_RAIO;
    bullet.amigo = true;

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    bullet.vx = k * nave.vx;
    bullet.vy = k * nave.vy;
    bullet.vz = k * nave.vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    double r = (nave.base.raio + bullet.raio)/modulo;
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
    if (nave.invencibilidade > 0) return;

    nave.base.hp -= dano;
    nave.invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (nave.base.hp <= 0) {
        nave.base.hp = 0;
        (nave.vidas)--;
        if (nave.vidas >= 0) criaNave(nave.base.z, nave.vidas);
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

    return (hipot(dx, dz) < somaRaios)
        && (abs(dy) < (nave.base.altura + foe->base.altura)/2);
}

/*------------------------------------------------------------------*/
 
void desenhaNave()
{
    const GLdouble NAVE_COR =
        255 - 190.0/INVENCIVEL_VIDA * nave.invencibilidade;

    glPushMatrix();
    glTranslated(nave.base.x, nave.base.y, nave.base.z);
    glRotated(nave.angHoriz * 180.0/PI, 0.0, 1.0, 0.0);
    glRotated(-nave.angVert * 180.0/PI, 1.0, 0.0, 0.0);
    glColorAlpha(NAVE_COR, NAVE_COR, 0, NAVE_COR);
    glutWireCone(nave.base.raio, nave.base.altura + 20, SLICES, STACKS);
    glPopMatrix();
}
