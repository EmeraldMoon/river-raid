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
    raio   = BALA_RAIO;
    altura = 2 * raio;
    dano   = BALA_DANO;
    amigo  = amigo;

    this->vx = vx;
    this->vy = vy;
    this->vz = vz;

    /* Posição inicial de projétil é exterior à unidade */
    double modulo = norma(vx, vy, vz);
    double k = (uni->getRaio() + raio)/modulo;
    x = uni->getX() + (k * vx);
    y = uni->getY() + (k * vy);
    z = uni->getZ() + (k * vz);
}

/*------------------------------------------------------------------*/

void Projetil::move()
{
    /* Efeito da gravidade */
    vy -= ACEL_GRAVIDADE;

    x += vx;
    y += vy;
    z += vz;
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
    if (colidiuCom(nave)) {
        nave->danifica(dano);
        return true;
    }
    /* Verificação de colisão com algum inimigo */
    std::vector<Inimigo> *inimigos = getListaInimigos();
    for (size_t i = 0; i < inimigos->size(); i++) {
        Inimigo *foe = &(*inimigos)[i];
        if (not colidiuCom(foe)) continue;
        if (amigo) {
            foe->danifica(dano);
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

double Projetil::getVx()    { return vx;    }
double Projetil::getVy()    { return vy;    }
double Projetil::getVz()    { return vz;    }
bool   Projetil::isAmigo()  { return amigo; }

/*------------------------------------------------------------------*/

void Projetil::desenha()
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Cores diferentes para tiros diferentes */
    if (amigo) setColor(LIGHT_GRAY);
    else             setColor(YELLOW);
  
    /* Desenha esfera na posição (x, y, z) */
    glTranslated(x, y, z);
    glutSolidSphere(raio, SLICES, STACKS);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

std::vector<Projetil> *getListaProjeteis()
{
    return &projeteis;
}

