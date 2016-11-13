#include <SFML/Graphics.hpp>

#include "tiro.hpp"
#include "base.hpp"
#include "cenario.hpp"
#include "jogo.hpp"
#include "cores.hpp"

/*---------------------*
 |   P R O J E T I L   |
 *---------------------*--------------------------------------------*/

Projetil::Projetil(Unidade &uni, double vx, double vy, double vz,
                   bool amigo)
{
    raio        = 3.0;
    altura      = 2 * raio;
    this->amigo = amigo;

    this->vx = vx;
    this->vy = vy;
    this->vz = vz;

    /* Posição inicial de projétil é exterior à unidade */
    double modulo = norma(vx, vy, vz);
    double k = (uni.getRaio() + raio)/modulo;
    x = uni.getX() + (k * vx);
    y = uni.getY() + (k * vy);
    z = uni.getZ() + (k * vz);
}

/*------------------------------------------------------------------*/

void Projetil::move()
{
    /* Efeito da gravidade */
    vy -= Cenario::ACEL_GRAVIDADE;

    x += vx;
    y += vy;
    z += vz;
}

/*------------------------------------------------------------------*/

void Projetil::desenha()
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Cores diferentes para tiros diferentes */
    if (amigo) setColor(LIGHT_GRAY);
    else       setColor(YELLOW);
  
    /* Desenha esfera na posição (x, y, z) */
    glTranslated(x, y, z);
    gluSphere(gluNewQuadric(), raio, SLICES, STACKS);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

double Projetil::getVx()   { return vx;    }
double Projetil::getVy()   { return vy;    }
double Projetil::getVz()   { return vz;    }
int    Projetil::getDano() { return dano;  }
bool   Projetil::isAmigo() { return amigo; }
