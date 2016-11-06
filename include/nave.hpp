/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada pelo jogador principal.                     |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include "base.hpp"
#include "item.hpp"

/*------------------------------------------------------------------*/

/* Atributos iniciais e/ou fixos */
#define NAVE_VIDAS    3
#define NAVE_VEL      4
#define NAVE_HPMAX  100
#define NAVE_COOL     6
#define NAVE_RAIO    20
#define NAVE_ALTURA  40

/* Ângulo de inclinação máxima da nave */
#define ANG_MAX (M_PI/6)

/* Nº de timesteps de invencibilidade */
#define INVENCIVEL_DANO 30  /* ao ser atingida */
#define INVENCIVEL_VIDA 90  /* ao perder uma vida */

/*------------------------------------------------------------------*/

/*
 *  Representa a nave do jogador.
 */
class Nave : public Unidade
{
private:
    /* Componentes de velocidade da nave */
    double vx, vy, vz;

    /* Número de chances do jogador. Se 0, "game over". */
    int vidas;

    /* Enquanto > 0, nave é imune a tiros e colisões */
    int invencibilidade;

    /* Serve como uma 2ª barra de vida para a nave, até ser destruída */
    int escudo;

    /* Pontuação do jogador */
    int score;

    void recria(int z, int nVidas);

    void atualizaDirecao(double *ang);

public:
    /* Guarda a única nave do jogo */
    static Nave *nave;

    /* Inclinações em relação ao eixo Oz */
    /* TEM QUE SER PRIVADO!!! */
    double angHoriz, angVert;

    /*
     *  Devolve a única nave do jogo.
     */
    static Nave *getNave();

    /*
     *  Cria uma nave, alocando memória e carregando modelo do OpenGL.
     *  Se godMode, então ela será eternamente invencível.
     */
    Nave(bool godMode);

    /*
     *  Atualiza a posição da nave em relação ao timestep anterior.
     *  As componentes horizontal e vertical são proporcionais
     *  à velocidade escalar da nave ao longo do eixo Oz.
     *  Também reduz tempo de invencibilidade.
     */
    void move();

    /*
     *  Decrementa invencibilidade até um mínimo de zero.
     */
    void atualizaInvencibilidade();

    /*
     *  Dispara um projétil na direção em que a nave estiver
     *  apontando e atualiza o tempo de recarga.
     */
    void dispara();

    /*
     *  Recebe uma quantidade de dano, remove a energia em questão
     *  da nave e ativa o período de invencibilidade.
     *  Caso hp <= 0, jogador perde uma vida.
     */
    void danifica(int dano);

    /*
     *  Ativa o efeito de determinado item na nave.
     */
    void ativaItem(Item *item);

    /*
     *  Desenha a representação visual da nave na tela.
     */
    void desenha();

    /* Getters */
    int getVidas();
    int getInvencibilidade();
    int getScore();

    /* Setters */
    void aumentaScore(int aumento);
};

/*------------------------------------------------------------------*/

/*
 *  Libera memória alocada para a nave.
 */
void liberaNave();
