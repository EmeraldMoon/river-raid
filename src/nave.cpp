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

    nave.corpo.raio      = NAVE_RAIO;
    nave.corpo.altura    = NAVE_ALTURA;
    nave.vz              = NAVE_VEL;
    nave.atribs.cooldown = NAVE_COOL;
    nave.score = 0;

    /* Define variável do módulo */
    godMode = _godMode;

    /* Começa em z == 0.0 */
    recriaNave(0.0, NAVE_VIDAS);
}

void recriaNave(int z, int nVidas)
{
    /* Coordenadas iniciais */
    nave.corpo.x = 0.0;
    nave.corpo.y = Y_MAX/2;
    nave.corpo.z = z;

    /* Aponta para o centro */
    nave.angHoriz = 0.0;
    nave.angVert  = 0.0;

    nave.vidas           = nVidas;
    nave.atribs.hp       = NAVE_HPMAX;
    nave.atribs.espera   = 0;
    nave.invencibilidade = INVENCIVEL_VIDA;
    nave.escudo          = 0;
}

/*------------------------------------------------------------------*/

static void atualizaDirecao(double *ang);

void moveNave()
{
    double *naveX = &nave.corpo.x;
    double *naveY = &nave.corpo.y;
    double *naveZ = &nave.corpo.z;

    /* Obtém vetores componentes */
    nave.vx = nave.vz * tan(nave.angHoriz);
    nave.vy = nave.vz * tan(nave.angVert);

    /* Atualiza posição por vetores */
    *naveX += nave.vx;
    *naveY += nave.vy;
    *naveZ += nave.vz;

    /* Impede que nave ultrapasse os limites do cenário */
    double raio   = nave.corpo.raio;
    double altura = nave.corpo.altura;
    if      (*naveX >  X_MAX -   raio) *naveX =  X_MAX - raio;
    else if (*naveX < -X_MAX +   raio) *naveX = -X_MAX + raio;
    if      (*naveY >  Y_MAX - altura) *naveY =  Y_MAX - altura;
    else if (*naveY <      0 + altura) *naveY =      0 + altura;

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
    bullet.corpo.raio = BALA_RAIO;
    bullet.dano = BALA_DANO;

    /* Módulo do vetor de velocidade */
    double modulo = norma(nave.vx, nave.vy, nave.vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    bullet.vx = k * nave.vx;
    bullet.vy = k * nave.vy;
    bullet.vz = k * nave.vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    double r = (nave.corpo.raio + bullet.corpo.raio)/modulo;
    bullet.corpo.x = nave.corpo.x + (r * nave.vx);
    bullet.corpo.y = nave.corpo.y + (r * nave.vy);
    bullet.corpo.z = nave.corpo.z + (r * nave.vz);

    /* Cria projétil e insere-o na lista */
    criaProjetil(&bullet);

    /* Reinicia contagem até próximo tiro */
    nave.atribs.espera = nave.atribs.cooldown;
}

/*------------------------------------------------------------------*/

void danificaNave(int dano)
{
    /* Se invencível neste instante, não toma dano */
    if (nave.invencibilidade > 0 || godMode) return;

    if (nave.escudo > 0) {
        nave.atribs.hp -=     dano/3;
        nave.escudo    -= 2 * dano/3;
    }
    else {
        nave.atribs.hp -= dano;
    }
    /* Nave fica invencível por um tempo pós-dano */
    nave.invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (nave.atribs.hp <= 0) {
        nave.atribs.hp = 0;
        if (--nave.vidas >= 0) recriaNave(nave.corpo.z, nave.vidas);
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
        glTranslated(nave.corpo.x, nave.corpo.y, nave.corpo.z);
        glRotated(rotacao, 1.0, 1.0, 0.0);
        setColorAlpha(DARK_BLUE, 255 * nave.escudo/(2.0 * NAVE_HPMAX));
        glutWireSphere(1.75 * NAVE_RAIO, SLICES, STACKS);
        glPopMatrix();
    }
    glPushMatrix();

    /* Posiciona nave rotacionada de acordo com ângulos de inclinação */
    glTranslated(nave.corpo.x, nave.corpo.y, nave.corpo.z);
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
        glScaled(2 * nave.corpo.raio, nave.corpo.altura,
                 2 * nave.corpo.raio);

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
