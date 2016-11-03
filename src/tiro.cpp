#include <cmath>  /* abs, sin, cos, atan2 */

#include "tiro.hpp"
#include "base.hpp"
#include "random.hpp"
#include "defesa.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de projéteis no campo de jogo */
std::vector<Projetil> projeteis;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

Projetil::Projetil(double z) : Corpo(z)
{
    this->raio   = BALA_RAIO;
    this->altura = 2 * this->raio;
    this->dano   = BALA_DANO;
}

/*------------------------------------------------------------------*/

static void calculaAngulo(double *a, double *b, double desvio);

/*
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 */
void Projetil::aplicaPrecisao(double precisao)
{
    double desvio = (1 - precisao) * DESVIO_MAX;

    calculaAngulo(&this->vx, &this->vz, desvio);  /* desvio horizontal */
    calculaAngulo(&this->vy, &this->vz, desvio);  /* desvio vertical   */
}

/*
 *  Recebe duas componentes, encontra seu ângulo em coordenadas 
 *  polares, modifica-o por meio de uma Normal e atualiza os valores.
 */
static void calculaAngulo(double *a, double *b, double desvio)
{
    double   v = hypot(*a, *b);
    double ang = atan2(*b, *a);
    ang = normal(ang, desvio);
    *a = v * cos(ang);
    *b = v * sin(ang); 
}

/*------------------------------------------------------------------*/

void Projetil::move()
{
    /* Efeito da gravidade */
    this->vy -= ACEL_GRAVIDADE;

    this->x += this->vx;
    this->y += this->vy;
    this->z += this->vz;
}

/*------------------------------------------------------------------*/

/*
 *  Considera-se que o projétil acertou um elemento caso a distância
 *  entre ambos seja MENOR que a soma dos raios (d < r + R).
 */
bool Projetil::verificaAcerto()
{
    Nave *nave = getNave();

    /* Verificação de colisão com a nave */
    if (this->colidiuCom(nave)) {
        nave->danifica(this->dano);
        return true;
    }
    /* Verificação de colisão com algum inimigo */
    std::vector<Inimigo> *inimigos = getListaInimigos();
    for (size_t i = 0; i < inimigos->size(); i++) {
        Inimigo *foe = &(*inimigos)[i];
        if (not this->colidiuCom(foe)) continue;
        if (this->amigo) {
            foe->hp -= this->dano;
            nave->score += foe->pontosAcerto;
            if (foe->hp <= 0) {
                inimigos->erase(inimigos->begin() + i);
                nave->score += foe->pontosDestruicao;
            } else {
                foe->tempoDano = FOE_TEMPO_DANO;
            }
        }
        return true;
    }
    return false;
}

/*------------------------------------------------------------------*/

void Projetil::desenha()
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Cores diferentes para tiros diferentes */
    if (this->amigo) setColor(LIGHT_GRAY);
    else             setColor(YELLOW);
  
    /* Desenha esfera na posição (x, y, z) */
    glTranslated(this->x, this->y, this->z);
    glutSolidSphere(this->raio, SLICES, STACKS);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

std::vector<Projetil> *getListaProjeteis()
{
    return &projeteis;
}

