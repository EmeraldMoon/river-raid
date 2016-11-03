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
static Nave nave(false);  // provisório

/* Modelo da nave */
static Modelo modelo;

/* Booleano que define invencibilidade perpétua */
static bool godMode;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

Nave::Nave(bool _godMode) : Unidade(0.0)
{
    /* Carrega modelo da nave */
    leVertices("nave.vert", &modelo);
    carregaTextura("silver.ppm", false, &modelo);

    this->raio     = NAVE_RAIO;
    this->altura   = NAVE_ALTURA;
    this->vz       = NAVE_VEL;
    this->cooldown = NAVE_COOL;
    this->score    = 0;

    /* Define variável do módulo */
    godMode = _godMode;

    /* Começa em z == 0.0 */
    this->recria(0.0, NAVE_VIDAS);
}

void Nave::recria(int z, int nVidas)
{
    /* Coordenadas iniciais */
    this->x = 0.0;
    this->y = Y_MAX/2;
    this->z = z;

    /* Aponta para o centro */
    this->angHoriz = 0.0;
    this->angVert  = 0.0;

    this->vidas           = nVidas;
    this->hp              = NAVE_HPMAX;
    this->espera          = 0;
    this->invencibilidade = INVENCIVEL_VIDA;
    this->escudo          = 0;
}

/*------------------------------------------------------------------*/

void Nave::move()
{
    /* Obtém vetores componentes */
    this->vx = this->vz * tan(this->angHoriz);
    this->vy = this->vz * tan(this->angVert);

    /* Atualiza posição por vetores */
    this->x += this->vx;
    this->y += this->vy;
    this->z += this->vz;

    /* Impede que nave ultrapasse os limites do cenário */
    if      (this->x >  X_MAX - this->raio  ) this->x =  X_MAX - this->raio;
    else if (this->x < -X_MAX + this->raio  ) this->x = -X_MAX + this->raio;
    if      (this->y >  Y_MAX - this->altura) this->y =  Y_MAX - this->altura;
    else if (this->y <      0 + this->altura) this->y =      0 + this->altura;

    /* Direção tende a voltar ao centro */
    this->atualizaDirecao(&(this->angHoriz));
    this->atualizaDirecao(&(this->angVert));
}

/*
 *  Recebe um ponteiro para um ângulo de inclinação da nave e diminui
 *  seu valor em módulo. Caso chegue a 0°, direção é mantida.
 */
void Nave::atualizaDirecao(double *ang)
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

void Nave::dispara()
{
    Projetil bullet(z);
    bullet.amigo  = true;

    /* Módulo do vetor de velocidade */
    double modulo = norma(this->vx, this->vy, this->vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    bullet.vx = k * this->vx;
    bullet.vy = k * this->vy;
    bullet.vz = k * this->vz;

    /* Posição inicial será colinear ao centro da nave e ao destino */
    double r = (this->raio + bullet.raio)/modulo;
    bullet.x = this->x + (r * this->vx);
    bullet.y = this->y + (r * this->vy);
    bullet.z = this->z + (r * this->vz);

    /* Insere projétil na lista */
    getListaProjeteis()->push_back(bullet);

    /* Reinicia contagem até próximo tiro */
    this->espera = this->cooldown;
}

/*------------------------------------------------------------------*/

void Nave::danifica(int dano)
{
    /* Se invencível neste instante, não toma dano */
    if (this->invencibilidade > 0 or godMode) return;

    if (this->escudo > 0) {
        this->hp     -=     dano/3;
        this->escudo -= 2 * dano/3;
    }
    else {
        this->hp -= dano;
    }
    /* Nave fica invencível por um tempo pós-dano */
    this->invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (this->hp <= 0) {
        this->hp = 0;
        if (--this->vidas >= 0) this->recria(this->z, this->vidas);
    }
}

/*------------------------------------------------------------------*/
 
void Nave::desenha()
{
    GLdouble naveCor = 255 - 190.0/INVENCIVEL_VIDA * this->invencibilidade;

    glDisable(GL_TEXTURE_2D);

    /* Se ativo, desenha escudo ao redor da nave */
    if (this->escudo > 0) {
        static double rotacao = 0;
        rotacao += PI/6;
        naveCor = 255;
        glPushMatrix();
        glTranslated(this->x, this->y, this->z);
        glRotated(rotacao, 1.0, 1.0, 0.0);
        setColorAlpha(DARK_BLUE, 255 * this->escudo/(2.0 * NAVE_HPMAX));
        glutWireSphere(1.75 * NAVE_RAIO, SLICES, STACKS);
        glPopMatrix();
    }
    glPushMatrix();

    /* Posiciona nave rotacionada de acordo com ângulos de inclinação */
    glTranslated(this->x, this->y, this->z);
    glRotated( this->angHoriz * 180.0/PI, 0.0, 1.0, 0.0);
    glRotated(-this->angVert  * 180.0/PI, 1.0, 0.0, 0.0);

    setColorAlpha(3 * naveCor, 3 * naveCor, 0, 3 * naveCor);
    if (estaEmPrimeiraPessoa()) {
        /* Exibe uma mira na tela */
        glutWireCone(0.25, 2, 4, 0); 
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, modelo.texturaId);
        glScaled(2 * this->raio, this->altura,
                 2 * this->raio);

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
