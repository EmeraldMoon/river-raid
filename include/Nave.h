/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada primariamente pelo jogador principal.       |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef NAVE_H
#define NAVE_H

#include <stdbool.h>
#include "Defesa.h"
#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Atributos iniciais e/ou fixos */
#define VIDAS_INI     3
#define NAVE_VEL      5
#define NAVE_HPMAX  100
#define NAVE_COOL     5
#define NAVE_RAIO    10
#define NAVE_ALTURA   5

/* Ângulo de inclinação máxima */
#define ANG_MAX PI/6

/* Taxas de alteração da direção a cada timestep */
#define ANG_MANUAL ANG_MAX/20  /* por comando do usuário */
#define ANG_AUTO   ANG_MAX/60  /* automático */

/* Nº de timesteps de invencibilidade */
#define INVENCIBIL_DANO 30  /* ao ser atingida */
#define INVENCIBIL_VIDA 90  /* ao perder uma vida */

/* Energia perdida caso ocorra colisão com inimigos */
#define DANO_COLISAO 20

/*
 *  Representa a nave do jogador.
 */
typedef struct nave Nave;
struct nave
{
    Corpo base;

    /* Número de chances do jogador */
    unsigned int vidas;

    /* Componentes da velocidade da nave */
    double vx, vy, vz;

    /* Inclinações em relação ao eixo Oz */
    double angX, angY;

    /* Guarda pontuação do jogador */
    unsigned int score;

    /* Enquanto > 0, nave é imune a tiros e colisões */
    unsigned int invencibilidade;
};

extern Nave nave;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Recebe a posição no eixo Oz da nave e um número de vidas.
 *  Inicializa os atributos da nave.
 */
void criaNave(int z, int nVidas);

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
 *  da nave e ativa o período de invencibilidade. Caso hp <= 0,
 *  jogador perde uma vida.
 */
void danificaNave(int dano);

/*
 *  Verifica se nave colidiu com um determinado inimigo,
 *  ou seja, se seus corpos entraram em contato.
 */
bool naveColidiu(Inimigo *foe);

#endif
