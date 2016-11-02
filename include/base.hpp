/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições de uso geral e estruturas comuns, a serem            |
 |  usadas em mais de um módulo do projeto.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdlib.h>   /* size_t */
#include <stdbool.h>  /* bool */

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Pi, nossa constante querida */
#define PI 3.14159265359

/* Calcula norma de um vetor */
#define norma(x, y, z) sqrt((x)*(x) + (y)*(y) + (z)*(z))

/*------------------------------------------------------------------*/

/*
 *  Estrutura básica de um elemento físico do jogo.
 *  A ser usada comumente entre todas as structs.
 */
typedef struct corpo Corpo;
struct corpo
{
    /* Posição do corpo no cenário */
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o início do cenário */

    /* Dimensões do corpo.
       No caso de esféricos, altura == 2 * raio. */
    double raio, altura;
};

/*
 *  Estrutura contendo alguns atributos comuns
 *  entre nave e inimigos.
 */
typedef struct atributos Atributos;
struct atributos
{
    /* Se (hp <= 0), elemento é destruído */
    int hp;

    /* Timesteps de espera entre um tiro e outro.
       cooldown é fixo, espera é decrementada de cooldown até 0. */
    int cooldown, espera;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Posiciona um corpo de forma randômica com posições
 *  aleatórias x e y e coordenada fixa z.
 */
void geraCorpo(Corpo *corpo, double z);

/*
 *  Verifica se ocorreu colisão entre dois corpos.
 *  Os dois corpos são considerados cilíndricos, para facilitar.
 */
bool ocorreuColisao(Corpo *a, Corpo *b);

/*
 *  Verifica se o corpo saiu por um dos limites da tela.
 *  naveZ refere-se à posição z da nave neste instante.
 */
bool corpoSaiu(Corpo *corpo, double naveZ);

/*
 *  Versão segura de malloc(). Caso não haja memória disponível,
 *  exibe uma mensagem de erro e encerra o programa.
 */
void *mallocSafe(size_t nbytes);
