/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada primariamente pelo jogador principal.       |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef NAVE_H
#define NAVE_H

#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Atributos iniciais e/ou fixos */
#define NAVE_VEL      5
#define VIDAS_INI     3
#define NAVE_HPMAX  100
#define NAVE_COOL     5
#define NAVE_RAIO    10
#define NAVE_ALTURA   5

/* Ângulo de inclinação máxima */
#define ANG_MAX PI/4

/* Taxas de alteração da direção a cada timestep */
#define ANG_MANUAL ANG_MAX/20  /* por comando do usuário */
#define ANG_AUTO   ANG_MAX/60  /* automático */

/*
 *  Representa a nave do jogador.
 */
typedef struct nave Nave;
struct nave
{
    Esqueleto base;

    /* Número de chances do jogador */
    int vidas;

    /* Componentes da velocidade da nave */
    double vx, vy, vz;

    /* Inclinações em relação ao eixo Oz */
    double angX, angY;
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

#endif
