#include <cmath>  /* abs, sin, cos, atan2 */

#include "tiro.hpp"
#include "base.hpp"
#include "nave.hpp"
#include "defesa.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de projéteis no campo de jogo */
std::vector<Projetil> projeteis;

/*---------------------*
 |   P R O J E T I L   |
 *---------------------*--------------------------------------------*/

Projetil::Projetil(Unidade *uni, double vx, double vy, double vz,
                   bool amigo)
{
    this->raio   = BALA_RAIO;
    this->altura = 2 * this->raio;
    this->dano   = BALA_DANO;
    this->amigo  = amigo;

    this->vx = vx;
    this->vy = vy;
    this->vz = vz;

    /* Posição inicial de projétil é exterior à unidade */
    double modulo = norma(vx, vy, vz);
    double k = (uni->getRaio() + this->raio)/modulo;
    this->x = uni->getX() + (k * vx);
    this->y = uni->getY() + (k * vy);
    this->z = uni->getZ() + (k * vz);
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
            foe->danifica(this->dano);
            nave->aumentaScore(foe->getPontosAcerto());
            if (foe->getHP() <= 0) {
                inimigos->erase(inimigos->begin() + i);
                nave->aumentaScore(foe->getPontosDestruicao());
            }
        }
        return true;
    }
    return false;
}

/*------------------------------------------------------------------*/

double Projetil::getVx()    { return this->vx;    }
double Projetil::getVy()    { return this->vy;    }
double Projetil::getVz()    { return this->vz;    }
bool   Projetil::isAmigo()  { return this->amigo; }

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

