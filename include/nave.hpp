/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada pelo jogador principal.                     |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include "base.hpp"
#include "item.hpp"

/*------------------------------------------------------------------*/

/*
 *  Representa a nave do jogador.
 */
class Nave : public Unidade
{
private:
    /* Timesteps de invencibilidade a cada vida nova */
    static constexpr int INVENCIVEL_VIDA = 90;

    /* Inclinações em relação ao eixo Oz */
    double angHoriz, angVert;

    /* Componentes de velocidade da nave */
    double vx, vy, vz = 4.0;

    /* Número de chances do jogador. Se 0, "game over". */
    int vidas;

    /* Enquanto > 0, nave é imune a tiros e colisões */
    int invencibilidade;

    /* Serve como uma 2ª barra de vida para a nave, até ser destruída */
    int escudo;

    /* Pontuação do jogador */
    int score = 0;

    void recria(int z, int nVidas);

    void atualizaDirecao(double &d, int sentido);

public:
    /*
     *  Cria uma nave, alocando memória e carregando modelo do OpenGL.
     *  Se godMode, então ela será eternamente invencível.
     */
    Nave(bool godMode);

    /*
     *  Atualiza a posição da nave em relação ao timestep anterior.
     *  As componentes horizontal e vertical são proporcionais
     *  à velocidade escalar da nave ao longo do eixo Oz.
     */
    void move();

    /*
     *  Dado um certo sentido, atualiza ângulo de inclinação.
     */
    void atualizaHorizontal(int sentido);
    void atualizaVertical(int sentido);

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
    void ativaItem(Item &item);

    /*
     *  Desenha a representação visual da nave na tela.
     */
    void desenha();

    /* Getters */
    int getAngHoriz();
    int getAngVert();
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
