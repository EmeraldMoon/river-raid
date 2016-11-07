#include <cmath>      /* hypot */
#include <algorithm>  /* find */

#include "base.hpp"
#include "random.hpp"
#include "nave.hpp"
#include "defesa.hpp"
#include "tiro.hpp"
#include "item.hpp"
#include "cenario.hpp"
#include "grafico.hpp"

/*---------------*
 |   L I S T A   |
 *---------------*--------------------------------------------------*/

template<typename T>
typename std::vector<T>::iterator Lista<T>::begin()
{
    return vec.begin();
}

template<typename T>
typename std::vector<T>::iterator Lista<T>::end()
{
    return vec.end();
}

template<typename T>
void Lista<T>::insere(T &objeto)
{
    vec.push_back(objeto);
}

template<typename T>
void Lista<T>::remove(T &objeto)
{
    /* Só é possível obter a posição deste jeito pois, por padrão,
       std::vector guarda seus elementos contiguamente na memória. */
    int pos = &objeto - &vec[0];
    vec.erase(vec.begin() + pos);
}

/* Indica quais tipos o template de Lista pode assumir */
template class Lista<Inimigo>;
template class Lista<Projetil>;
template class Lista<Item>;

/*---------------*
 |   C O R P O   |
 *---------------*--------------------------------------------------*/

Corpo::Corpo() { }

Corpo::Corpo(double z)
{
    x = Cenario::X_MAX * uniforme(-0.88, 0.88);
    y = uniforme(Cenario::Y_MAX/8, Cenario::Y_MAX/2);
    this->z = z;
}

/*
 *  Em tese deveríamos fazer uma verificação diferente, na vertical,
 *  para caso um dos corpos fosse esférico, mas isso envolveria
 *  cálculos chatos (como verificar se esfera está contida
 *  em extremos do cilindro). Então, simplificamos.
 */
bool Corpo::colidiuCom(Corpo &corpo)
{
    double dx = x - corpo.getX();
    double dy = y - corpo.getY();
    double dz = z - corpo.getZ();
    double somaRaios = raio + corpo.raio;

    /* Evita cálculos desnecessários */
    if (dx >= somaRaios or dz >= somaRaios) return false;

    /* Distância vertical deve ser menor que soma das semi-alutras.
       Circunferências horizontais devem ser secantes. */
    return (abs(dy) < (altura + corpo.altura)/2
                and hypot(dx, dz) < somaRaios);
}

bool Corpo::saiu()
{
    double naveZ = Cenario::get().nave.getZ();

    /* O corpo saiu por um dos limites da tela (x, y ou z)?
       Caso a respostas seja sim, então o corpo saiu do jogo. */
    return (abs(x) > Cenario::X_MAX)
        or (y < 0)
        or (z < naveZ - DIST_CAMERA or z > naveZ + Cenario::Z_DIST);
}

double Corpo::getX()      { return x;      }
double Corpo::getY()      { return y;      }
double Corpo::getZ()      { return z;      }
double Corpo::getRaio()   { return raio;   }
double Corpo::getAltura() { return altura; }

/*------------------*
 |   U N I D A D E  |
 *------------------*-----------------------------------------------*/

Unidade::Unidade(double z) : Corpo(z) { }

int Unidade::getHPMax()    { return hpMax;    }
int Unidade::getHP()       { return hp;       }
int Unidade::getCooldown() { return cooldown; }
int Unidade::getEspera()   { return espera;   }

int Unidade::reduzEspera() { return --espera; }
