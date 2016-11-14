/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições de uso geral e estruturas comuns, a serem            |
 |  usadas em mais de um módulo do projeto.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <cmath>
#include <list>

/*------------------------------------------------------------------*/

/* Pi, nossa constante querida */
#define M_PI 3.14159265358979323846

/* Calcula norma de um vetor de 3 dimensões */
#define norma(x, y, z) sqrt((x)*(x) + (y)*(y) + (z)*(z))

/*------------------------------------------------------------------*/

/*
 *  Representa uma lista de elementos de um dado tipo.
 */
template<typename T>
class Lista {
private:
    /* Container que é a lista em si */
    std::list<T> list;

public:
    /*
     *  Métodos iteradores, necessários para usar ranged for.
     */
    typename std::list<T>::iterator begin();
    typename std::list<T>::iterator   end();

    /*
     *  Insere novo objeto na lista.
     */
    void insere(T &objeto);

    /*
     *  Remove objeto apontado atualmente pelo iterador 'it'.
     *  Retrocede iterador ao item anterior ao excluído.
     */
    void remove(typename std::list<T>::iterator &it);
};

/*------------------------------------------------------------------*/

/*
 *  Superclasse básica de um elemento físico do jogo.
 *  A ser usada comumente entre todos os objetos.
 */
class Corpo
{
protected:
    /* Posição do corpo no cenário */
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o ponto inicial do jogo */

    /* Dimensões do corpo.
       No caso de esféricos, altura == 2 * raio. */
    double raio, altura;

public:
    /*
     *  Apenas um construtor vazio.
     */
    Corpo();

    /*
     *  Gera um corpo de forma randômica com posições
     *  aleatórias x e y e coordenada fixa z.
     */
    Corpo(double z);

    /*
     *  Verifica se ocorreu colisão entre dois corpos.
     *  Os corpos são considerados cilíndricos, para facilitar.
     */
    bool colidiuCom(Corpo &corpo);

    /*
     *  Verifica se o corpo saiu por um dos limites da tela.
     */
    bool saiu();

    /* Getters */
    double getX();
    double getY();
    double getZ();
    double getRaio();
    double getAltura();
};

/*------------------------------------------------------------------*/

/*
 *  Unidade de batalha do jogo, com hp e tiros recarregáveis.
 *  Nave e inimigos são subclasses desta.
 */
class Unidade : public Corpo
{
protected:
    /* Quantidade máxima e atual de HP.
       Se hp == 0, elemento é destruído. */
    int hpMax, hp;

    /* Timesteps de espera entre um tiro e outro.
       cooldown é fixo, espera é decrementada de cooldown até 0. */
    int cooldown, espera;

public:
    /*
     *  Apenas chama o construtor de Corpo.
     */
    Unidade(double z);

    /* Getters */
    int getHPMax();
    int getHP();
    int getCooldown();
    int getEspera();

    /* Setters */
    int reduzEspera();
};
