#include <math.h>  /* tan, abs */
#include <GL/freeglut.h>

#include "Nave.h"
#include "Tiro.h"
#include "Cenario.h"
#include "Textura.h"
#include "Modelo.h"
#include "Cores.h"
#include "Grafico.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Ponteiro para estrutura da nave */
// static Nave *nave; provisório \/
Nave *nave;

/* Matriz representando vértices da nave */
static GLdouble naveVertices[NAVE_NUM_VERTICES][3];

/* Booleano que define invencibilidade perpétua */
static bool godMode;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

Nave *carregaNave(bool _godMode)
{
    /* Aloca memória para a nave */
    nave = mallocSafe(sizeof *nave);

    /* Carrega vértices do modelo */
    leVetores(naveVertices, NAVE_NUM_VERTICES, NAVE_MODELO_VERTICES);

    nave->corpo.raio      = NAVE_RAIO;
    nave->corpo.altura    = NAVE_ALTURA;
    nave->vz              = NAVE_VEL;
    nave->atribs.cooldown = NAVE_COOL;
    nave->score = 0;

    /* Define variável do módulo */
    godMode = _godMode;

    /* Começa em z == 0.0 */
    recriaNave(0.0, NAVE_VIDAS);

    return nave;
}

void recriaNave(int z, int nVidas)
{
    /* Coordenadas iniciais */
    nave->corpo.x = 0.0;
    nave->corpo.y = Y_MAX/2;
    nave->corpo.z = z;

    /* Aponta para o centro */
    nave->angHoriz = 0.0;
    nave->angVert  = 0.0;

    nave->vidas           = nVidas;
    nave->atribs.hp       = NAVE_HPMAX;
    nave->atribs.espera   = 0;
    nave->invencibilidade = INVENCIVEL_VIDA;
    nave->escudo          = 0;
}

/*------------------------------------------------------------------*/

static void atualizaDirecao(double *ang);

void moveNave()
{
    double *naveX = &nave->corpo.x;
    double *naveY = &nave->corpo.y;
    double *naveZ = &nave->corpo.z;

    /* Obtém vetores componentes */
    nave->vx = nave->vz * tan(nave->angHoriz);
    nave->vy = nave->vz * tan(nave->angVert);

    /* Atualiza posição por vetores */
    *naveX += nave->vx;
    *naveY += nave->vy;
    *naveZ += nave->vz;

    /* Impede que nave ultrapasse os limites do cenário */
    double raio   = nave->corpo.raio;
    double altura = nave->corpo.altura;
    if      (*naveX >  X_MAX -   raio) *naveX =  X_MAX - raio;
    else if (*naveX < -X_MAX +   raio) *naveX = -X_MAX + raio;
    if      (*naveY >  Y_MAX - altura) *naveY =  Y_MAX - altura;
    else if (*naveY <      0 + altura) *naveY =      0 + altura;

    /* Direção tende a voltar ao centro */
    atualizaDirecao(&(nave->angHoriz));
    atualizaDirecao(&(nave->angVert));
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
    /* Cria objeto do projétil */
    Projetil *bullet = criaProjetil();
    bullet->amigo = true;

    /* Módulo do vetor de velocidade */
    double modulo = norma(nave->vx, nave->vy, nave->vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    bullet->vx = k * nave->vx;
    bullet->vy = k * nave->vy;
    bullet->vz = k * nave->vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    double r = (nave->corpo.raio + bullet->corpo.raio)/modulo;
    bullet->corpo.x = nave->corpo.x + (r * nave->vx);
    bullet->corpo.y = nave->corpo.y + (r * nave->vy);
    bullet->corpo.z = nave->corpo.z + (r * nave->vz);

    /* Reinicia contagem até próximo tiro */
    nave->atribs.espera = nave->atribs.cooldown;
}

/*------------------------------------------------------------------*/

void danificaNave(int dano)
{
    /* Se invencível neste instante, não toma dano */
    if (nave->invencibilidade > 0 || godMode) return;

    if (nave->escudo > 0) {
        nave->atribs.hp -=     dano/3;
        nave->escudo    -= 2 * dano/3;
    }
    else {
        nave->atribs.hp -= dano;
    }
    /* Nave fica invencível por um tempo pós-dano */
    nave->invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (nave->atribs.hp <= 0) {
        nave->atribs.hp = 0;
        if (--nave->vidas >= 0) recriaNave(nave->corpo.z, nave->vidas);
    }
}

/*------------------------------------------------------------------*/
 
void desenhaNave()
{
    static double rotacao = 0;
    rotacao += PI/6;

    GLdouble NAVE_COR =
        255 - 190.0/INVENCIVEL_VIDA * nave->invencibilidade;

    if (nave->escudo > 0 && !estaEmPrimeiraPessoa()) {
        NAVE_COR = 255;
        glPushMatrix();
        glColorAlpha(DARK_BLUE, 250 * nave->escudo/(2.0*NAVE_HPMAX));
        glDisable(GL_TEXTURE_2D);
        glTranslated(nave->corpo.x, nave->corpo.y, nave->corpo.z);
        glRotated(rotacao, 1.0, 1.0, 0.0);
        glutWireSphere(1.75*NAVE_RAIO, SLICES, STACKS);
        glPopMatrix();
    }
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslated(nave->corpo.x, nave->corpo.y, nave->corpo.z);
    glRotated(nave->angHoriz * 180.0/PI, 0.0, 1.0, 0.0);
    glRotated(-nave->angVert * 180.0/PI, 1.0, 0.0, 0.0);

    glColorAlpha(3*NAVE_COR, 3*NAVE_COR, 0, 3*NAVE_COR);
    if (estaEmPrimeiraPessoa()) {
        glDisable(GL_TEXTURE_2D);
        glutWireCone(0.25, 2, 4, 0);
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, naveTextura);
        glScaled(20.0, 40.0, 25.0);

        glEnableClientState(GL_VERTEX_ARRAY);

        /* Desenha os vértices do arquivo */
        glVertexPointer(3, GL_DOUBLE, 0, naveVertices);
        glDrawArrays(GL_TRIANGLES, 0, NAVE_NUM_VERTICES);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Nave *getNave()
{
    return nave;
}
