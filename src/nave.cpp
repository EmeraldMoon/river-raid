#include <cmath>  /* tan, abs */
#include <GL/freeglut.h>

#include "nave.hpp"
#include "tiro.hpp"
#include "item.hpp"
#include "cenario.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Ponteiro para estrutura da nave */
static Nave *nave;

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

    raio     = NAVE_RAIO;
    altura   = NAVE_ALTURA;
    vz       = NAVE_VEL;
    cooldown = NAVE_COOL;
    score    = 0;

    /* Define variável do módulo */
    godMode = _godMode;

    /* Começa em z == 0.0 */
    recria(0.0, NAVE_VIDAS);

    nave = this;
}

/*
 *  Recebe a posição no eixo Oz da nave e um número de vidas.
 *  Reinicializa os atributos variáveis da nave.
 */
void Nave::recria(int z, int nVidas)
{
    /* Coordenadas iniciais */
    x = 0.0;
    y = Y_MAX/2;
    z = z;

    /* Aponta para o centro */
    angHoriz = 0.0;
    angVert  = 0.0;

    vidas           = nVidas;
    hp              = NAVE_HPMAX;
    espera          = 0;
    invencibilidade = INVENCIVEL_VIDA;
    escudo          = 0;
}

/*------------------------------------------------------------------*/

void Nave::move()
{
    /* Obtém vetores componentes */
    vx = vz * tan(angHoriz);
    vy = vz * tan(angVert);

    /* Atualiza posição por vetores */
    x += vx;
    y += vy;
    z += vz;

    /* Impede que nave ultrapasse os limites do cenário */
    if      (x >  X_MAX - raio  ) x =  X_MAX - raio;
    else if (x < -X_MAX + raio  ) x = -X_MAX + raio;
    if      (y >  Y_MAX - altura) y =  Y_MAX - altura;
    else if (y <      0 + altura) y =      0 + altura;

    /* Direção tende a voltar ao centro */
    atualizaDirecao(&(angHoriz));
    atualizaDirecao(&(angVert));
}

void Nave::atualizaInvencibilidade()
{
    if (invencibilidade > 0) invencibilidade--;
}

/*------------------------------------------------------------------*/

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
    bool amigo = true;

    /* Módulo do vetor de velocidade */
    double modulo = norma(vx, vy, vz);

    /* Componentes da velocidade da bala são proporcionais à nave */
    double k = BALA_VEL/modulo;
    double vx = k * this->vx;
    double vy = k * this->vy;
    double vz = k * this->vz;

    /* Cria projétil e o insere na lista */
    Projetil bullet(this, vx, vy, vz, amigo);
    getListaProjeteis()->push_back(bullet);

    /* Reinicia contagem até próximo tiro */
    espera = cooldown;
}

/*------------------------------------------------------------------*/

void Nave::danifica(int dano)
{
    /* Se invencível neste instante, não toma dano */
    if (invencibilidade > 0 or godMode) return;

    if (escudo > 0) {
        hp     -=     dano/3;
        escudo -= 2 * dano/3;
    }
    else {
        hp -= dano;
    }
    /* Nave fica invencível por um tempo pós-dano */
    invencibilidade = INVENCIVEL_DANO;

    /* Verifica se nave perdeu vida */
    if (hp <= 0) {
        hp = 0;
        if (--vidas >= 0) recria(z, vidas);
    }
}

/*------------------------------------------------------------------*/

void Nave::ativaItem(Item *item)
{
    switch (item->getTipo()) {
    case HP:
        hp += NAVE_HPMAX/6;
        if (hp > NAVE_HPMAX) hp = NAVE_HPMAX;
        break;
    case VIDA:
        vidas++;
        break;
    case ESCUDO:
        escudo = 2 * NAVE_HPMAX;
        break;
    }
}

/*------------------------------------------------------------------*/
 
void Nave::desenha()
{
    GLdouble naveCor = 255 - 190.0/INVENCIVEL_VIDA * invencibilidade;

    glDisable(GL_TEXTURE_2D);

    /* Se ativo, desenha escudo ao redor da nave */
    if (escudo > 0) {
        static double rotacao = 0;
        rotacao += PI/6;
        naveCor = 255;
        glPushMatrix();
        glTranslated(x, y, z);
        glRotated(rotacao, 1.0, 1.0, 0.0);
        setColorAlpha(DARK_BLUE, 255 * escudo/(2.0 * NAVE_HPMAX));
        glutWireSphere(1.75 * NAVE_RAIO, SLICES, STACKS);
        glPopMatrix();
    }
    glPushMatrix();

    /* Posiciona nave rotacionada de acordo com ângulos de inclinação */
    glTranslated(x, y, z);
    glRotated( angHoriz * 180.0/PI, 0.0, 1.0, 0.0);
    glRotated(-angVert  * 180.0/PI, 1.0, 0.0, 0.0);

    setColorAlpha(3 * naveCor, 3 * naveCor, 0, 3 * naveCor);
    if (estaEmPrimeiraPessoa()) {
        /* Exibe uma mira na tela */
        glutWireCone(0.25, 2, 4, 0); 
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, modelo.texturaId);
        glScaled(2 * raio, altura,
                 2 * raio);

        /* Desenha os vértices do arquivo */
        glVertexPointer(3, GL_DOUBLE, 0, modelo.coords);
        glDrawArrays(GL_TRIANGLES, 0, modelo.numVertices);
    }
    glPopMatrix();
}

/*------------------------------------------------------------------*/

int Nave::getVidas()           { return vidas;           }
int Nave::getInvencibilidade() { return invencibilidade; }
int Nave::getScore()           { return score;           }

void Nave::aumentaScore(int aumento) { score += aumento; }

/*------------------------------------------------------------------*/

Nave *getNave()
{
    return nave;
}

/*------------------------------------------------------------------*/

void liberaNave()
{
    liberaTextura(&modelo);
    liberaVertices(&modelo);
}
