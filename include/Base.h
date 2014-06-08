/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições de uso geral e estruturas comuns, a serem            |
 |  usadas em mais de um módulo do projeto.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdlib.h>  /* size_t, malloc, exit */

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Pi, nossa constante querida */
#define PI 3.14159265359

/* Calcula quadrado de x */
#define sq(x) (pow((x), 2))

/* Calcula norma de um vetor */
#define norma(x, y, z) (sqrt(sq(x) + sq(y) + sq(z)))

/* Calcula hipotenusa de um triângulo */
#define hipot(x, y) (sqrt(sq(x) + sq(y)))

/*
 *  Estrutura básica de um elemento corpóreo do jogo.
 *  A ser usada comumente entre nave e seus inimigos.
 */
typedef struct corpo Corpo;
struct corpo
{
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o início do cenário */
    
    /* Se (hp <= 0), elemento é destruído */
    int hp;
    
    /* Timesteps de espera entre um tiro e outro */
    unsigned int cooldown;  /* valor máximo, fixo */
    unsigned int espera;    /* sofre decremento até chegar a 0 */

    /* Corpo de colisão é um CILINDRO reto */
    unsigned int raio;
    unsigned int altura;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Versão segura de malloc(). Caso não haja memória disponível,
 *  exibe uma mensagem de erro e encerra o programa.
 */
void * mallocSafe(size_t nbytes);
