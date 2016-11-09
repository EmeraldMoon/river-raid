#include <cmath>  /* sqrt */

#include "defesa.hpp"
#include "random.hpp"
#include "tiro.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "modelo.hpp"
#include "cores.hpp"

/*------------------------------------------------------------------*/

/* Modelo da nave */
static Modelo modelo;

/*-------------------*
 |   I N I M I G O   |
 *-------------------*----------------------------------------------*/

void carregaInimigos()
{
    /* Carrega modelo da nave */
    leVertices("defesa.vert", modelo);
    carregaTextura("magma.ppm", false, modelo);
}

/*------------------------------------------------------------------*/

Inimigo::Inimigo(double z) : Unidade(z)
{
    /* Dimensões do corpo */
    raio   = 40.0;
    altura = 2 * y;

    /* Atributos restantes */
    hpMax    = hp     = uniforme(50, 100);
    cooldown = espera = uniforme(50, 100);
    precisao          = uniforme(0.5, 1.0);
}

/*------------------------------------------------------------------*/

void Inimigo::dispara(Nave &nave)
{
    bool amigo = false;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    double dx =  nave.getX() - x;
    double dy =  nave.getY() - y;
    double dz = (nave.getZ() + nave.getRaio()) - z;
    double d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    double k = Projetil::VEL_PADRAO/d;
    double vx = k * dx;
    double vy = k * dy;
    double vz = k * dz;

    /* Aplica desvio de precisão */
    aplicaPrecisao(vx, vy, vz);

    /* Cria projétil e o insere na lista */
    Projetil bullet(*this, vx, vy, vz, amigo);
    Cenario::get().projeteis.insere(bullet);

    /* Reinicia contagem até próximo tiro */
    espera = cooldown;
}

static void calculaAngulo(double &a, double &b, double desvio);

/*
 *  Aplica no vetor direção dois desvios em graus, um horizontal
 *  e outro vertical, ambos inversamente proporcionais à precisão.
 *
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 */
void Inimigo::aplicaPrecisao(double &dx, double &dy, double &dz)
{
    static constexpr double DESVIO_MAX = M_PI/8;
    double desvio = (1 - precisao) * DESVIO_MAX;

    calculaAngulo(dx, dz, desvio);  /* desvio horizontal */
    calculaAngulo(dy, dz, desvio);  /* desvio vertical   */
}

/*
 *  Recebe duas componentes, encontra seu ângulo em coordenadas 
 *  polares, modifica-o por meio de uma Normal e atualiza os valores.
 */
static void calculaAngulo(double &d1, double &d2, double desvio)
{
    double   d = hypot(d1, d2);
    double ang = atan2(d2, d1);
    ang = normal(ang, desvio);
    d1 = d * cos(ang);
    d2 = d * sin(ang); 
}

/*------------------------------------------------------------------*/

void Inimigo::danifica(int dano)
{
    hp -= dano;
    tempoDano = 30;
}

/*------------------------------------------------------------------*/

void Inimigo::desenha()
{
    if (tempoDano > 0) tempoDano--;

    glPushMatrix();
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    /* Aplica cor e textura */
    setColor(WHITE);
    glBindTexture(GL_TEXTURE_2D,
                  (tempoDano == 0) ? modelo.texturaId : 0);

    /* Posiciona e dimensiona inimigo */
    glTranslated(x, 0.0, z);
    glScaled(16 * raio, altura, 16 * raio);

    /* Desenha modelo baseado em vértices e normais */
    glVertexPointer(3, GL_DOUBLE, 0, modelo.coords.data());
    glDrawArrays(GL_TRIANGLES,    0, modelo.coords.size()/3);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

int    Inimigo::getDanoColisao()      { return DANO_COLISAO;      }
int    Inimigo::getPontosAcerto()     { return PONTOS_ACERTO;     }
int    Inimigo::getPontosDestruicao() { return PONTOS_DESTRUICAO; }
double Inimigo::getPrecisao()         { return precisao;          }

/*------------------------------------------------------------------*/

void liberaInimigos()
{
    liberaTextura(modelo);
}
