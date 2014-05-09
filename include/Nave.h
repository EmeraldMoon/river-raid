/*------------------------------------------------------------------*
 |                                                                  |
 |  Nave que será usada primariamente pelo jogador principal.       |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef NAVE_H
#define NAVE_H

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

#define VEL_INI     5  /* velocidade inicial */
#define NAVE_RAIO  10  /* para testes, nave será esférica */
#define NAVE_HP   100

/* Tipo para representar radianos */
typedef double Rad;

typedef struct nave Nave;
struct nave
{
    int x;  /* posição horizontal (centro == 0) */
    int y;  /* altura da nave */
    int z;  /* distância percorrida */
     
    /* Inclinações em relação ao eixo Oz */
    Rad horizontal;
    Rad vertical;

    /* Velocidade escalar ao longo do cenário */
    int velocidade;

    /* Se (hp <= 0), perde a vida */
    int hp;
};

extern Nave nave;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa atributos da nave.
 */
void criaNave();

/*
 *  Atualiza a posição da nave em relação ao timestep anterior.
 *  As componentes horizontal e vertical são proporcionais
 *  à velocidade escalar da nave ao longo do eixo Oz.
 */
void moveNave();

#endif
