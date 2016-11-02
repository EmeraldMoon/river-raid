#include <cmath>  /* tan, abs */
#include <GL/freeglut.h>

#include "nave.hpp"
#include "tiro.hpp"
#include "cenario.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Ponteiro para estrutura da nave */
static Nave nave;

/* Modelo da nave */
static Modelo modelo;

/* Booleano que define invencibilidade perpétua */
static bool godMode;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaNave(bool _godMode)
{
    /* Carrega modelo da nave */
    leVertices("Nave.vert", &modelo);
    carregaTextura("silver.ppm", false, &modelo);

    nave.raio     = NAVE_RAIO;
    nave.altura   = NAVE_ALTURA;
    nave.vz       = NAVE_VEL;
    nave.cooldown = NAVE_COOL;
    nave.score = 0;

    /* Define variável do módulo */
    godMode = _godMode;

    /* Começa em z == 0.0 */
    recriaNave(0.0, NAVE_VIDAS);
}

void recriaNave(int z, int nVidas)
{
    /* Coordenadas iniciais */
    nave.x = 0.0;
    nave.y = Y_MAX/2;
    nave.z = z;

    /* Aponta para o centro */
    nave.angHoriz = 0.0;
    nave.angVert  = 0.0;

    nave.vidas           = nVidas;
    nave.hp              = NAVE_HPMAX;
    nave.espera          = 0;
    nave.invencibilidade = INVENCIVEL_VIDA;
    nave.escudo          = 0;
}

/*------------------------------------------------------------------*/

static void atualizaDirecao(double *ang);

void moveNave()
{
    /* Obtém vetores componentes */
    nave.vx = nave.vz * tan(nave.angHoriz);
    nave.vy = nave.vz * tan(nave.angVert);

    /* Atualiza posição por vetores */
    nave.x += nave.vx;
    nave.y += nave.vy;
    nave.z += nave.vz;

    /* Impede que nave ultrapasse os limites do cenário */
    if      (nave.x >  X_MAX - nave.raio  ) nave.x =  X_MAX - nave.raio;
    else if (nave.x < -X_MAX + nave.raio  ) nave.x = -X_MAX + nave.raio;
    if      (nave.y >  Y_MAX - nave.altura) nave.y =  Y_MAX - nave.altura;
    else if (nave.y <      0 + nave.altura) nave.y =      0 + nave.altura;

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
    /* Taxa de alteração automática por timestep */
    static const double ANG_AUTO = ANG_MAX/60;

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
    bullet.amigo = true;
    bullet.raio = BALA_RAIO;
    bullet.dano = BALA_DANO;

    /* Módulo do vetor de velocidade */
    double modulo = norma(nave.vx, nave.vy, nave.vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    bullet.vx = k * nave.vx;
    bullet.vy = k * nave.vy;
    bullet.vz = k * nave.vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    double r = (nave.raio + bullet.raio)/modulo;
    bullet.x = nave.x + (r * nave.vx);
    bullet.y = nave.y + (r * nave.vy);
    bullet.z = nave.z + (r * nave.vz);

    /* Cria projétil e insere-o na lista */
    criaProjetil(&bullet);

    /* Reinicia contagem até próximo tiro */
    nave.espera = nave.cooldown;
}

/*------------------------------------------------------------------*/

void danificaNave(int dano)
{
    /* Se invencível neste instante, não toma dano */
    if (nave.invencibilidade > 0 || godMode) return;

    if (nave.escudo > 0) {
        nave.hp -=     dano/3;
        nave.escudo    -= 2 * dano/3;
    }
    else {
        nave.hp -= dano;
    }
    /* Nave fica invencível por um tempo pós-dano */
    nave.invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (nave.hp <= 0) {
        nave.hp = 0;
        if (--nave.vidas >= 0) recriaNave(nave.z, nave.vidas);
    }
}

/*------------------------------------------------------------------*/
 
void desenhaNave()
{
    GLdouble naveCor = 255 - 190.0/INVENCIVEL_VIDA * nave.invencibilidade;

    glDisable(GL_TEXTURE_2D);

    /* Se ativo, desenha escudo ao redor da nave */
    if (nave.escudo > 0) {
        static double rotacao = 0;
        rotacao += PI/6;
        naveCor = 255;
        glPushMatrix();
        glTranslated(nave.x, nave.y, nave.z);
        glRotated(rotacao, 1.0, 1.0, 0.0);
        setColorAlpha(DARK_BLUE, 255 * nave.escudo/(2.0 * NAVE_HPMAX));
        glutWireSphere(1.75 * NAVE_RAIO, SLICES, STACKS);
        glPopMatrix();
    }
    glPushMatrix();

    /* Posiciona nave rotacionada de acordo com ângulos de inclinação */
    glTranslated(nave.x, nave.y, nave.z);
    glRotated( nave.angHoriz * 180.0/PI, 0.0, 1.0, 0.0);
    glRotated(-nave.angVert  * 180.0/PI, 1.0, 0.0, 0.0);

    setColorAlpha(3 * naveCor, 3 * naveCor, 0, 3 * naveCor);
    if (estaEmPrimeiraPessoa()) {
        /* Exibe uma mira na tela */
        glutWireCone(0.25, 2, 4, 0); 
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, modelo.texturaId);
        glScaled(2 * nave.raio, nave.altura,
                 2 * nave.raio);

        /* Desenha os vértices do arquivo */
        glVertexPointer(3, GL_DOUBLE, 0, modelo.coords);
        glDrawArrays(GL_TRIANGLES, 0, modelo.numVertices);
    }
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Nave *getNave()
{
    return &nave;
}

/*------------------------------------------------------------------*/

void liberaNave()
{
    liberaTextura(&modelo);
    liberaVertices(&modelo);
}
