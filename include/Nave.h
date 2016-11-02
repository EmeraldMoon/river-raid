/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada pelo jogador principal.                     |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>

#include "Base.h"

/*-------------------------*----------------------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Atributos iniciais e/ou fixos */
#define NAVE_VIDAS    3
#define NAVE_VEL      4
#define NAVE_HPMAX  100
#define NAVE_COOL     6
#define NAVE_RAIO    20
#define NAVE_ALTURA  40

/* Ângulo de inclinação máxima da nave */
#define ANG_MAX PI/6

/* Nº de timesteps de invencibilidade */
#define INVENCIVEL_DANO 30  /* ao ser atingida */
#define INVENCIVEL_VIDA 90  /* ao perder uma vida */

/*------------------------------------------------------------------*/

/*
 *  Representa a nave do jogador.
 */
typedef struct nave Nave;
struct nave
{
    /* Corpo cilíndrico da nave */
    Corpo corpo;

    /* Componentes de velocidade da nave */
    double vx, vy, vz;

    /* Inclinações em relação ao eixo Oz */
    double angHoriz, angVert;

    /* Número de chances do jogador. Se 0, "game over". */
    int vidas;

    /* HP e cooldown */
    Atributos atribs;

    /* Enquanto > 0, nave é imune a tiros e colisões */
    int invencibilidade;

    /* Serve como uma 2ª barra de vida para a nave, até ser destruída */
    int escudo;

    /* Pontuação do jogador */
    int score;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Cria uma nave, alocando memória e carregando modelo do OpenGL.
 *  Se godMode, então ela será eternamente invencível.
 */
void carregaNave(bool godMode);

/*
 *  Recebe a posição no eixo Oz da nave e um número de vidas.
 *  Reinicializa os atributos variáveis da nave.
 */
void recriaNave(int z, int nVidas);

/*
 *  Atualiza a posição da nave em relação ao timestep anterior.
 *  As componentes horizontal e vertical são proporcionais
 *  à velocidade escalar da nave ao longo do eixo Oz.
 */
void moveNave();

/*
 *  Dispara um projétil na direção em que a nave estiver
 *  apontando e atualiza o tempo de recarga.
 */
void naveDispara();

/*
 *  Recebe uma quantidade de dano, remove a energia em questão
 *  da nave e ativa o período de invencibilidade.
 *  Caso hp <= 0, jogador perde uma vida.
 */
void danificaNave(int dano);

/*
 *  Desenha a representação visual da nave na tela.
 */
void desenhaNave();

/*
 *  Devolve um ponteiro para a nave.
 */
Nave *getNave();

/*
 *  Libera memória alocada para a nave.
 */
void liberaNave();
