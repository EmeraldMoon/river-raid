/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação dos elementos de defesa, os inimigos do jogo.       |
 |  Cuida de geração, remoção, disparo de projéteis, etc.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef DEFESA_H
#define DEFESA_H

#include <stdbool.h>
#include "Lista.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Energia total padrão */
#define FOE_HP 100

typedef struct inimigo Inimigo;
struct inimigo
{
    /* Posição do inimigo (x,y,z) */
    int x;
    int y;
    int z;

    /* Probabilidade do projétil acertar a nave */
    double precisao;

    /* Se (hp <= 0), inimigo é destruído */
    int hp;

    /* Timesteps de espera entre um tiro e outro */
    unsigned int cooldown;  /* valor máximo, fixo */
    unsigned int espera;    /* sofre decremento até chegar a 0 */
};

extern Lista *inimigos;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Insere um inimigo na respectiva lista.
 */
void criaInimigo(Inimigo foe);

/*
 *  Dispara um projétil e atualiza o tempo de recarga,
 *  gerando um período de espera até o próximo.
 */
void dispara(Inimigo *foe);

/*
 *  Verifica se o inimigo passou pela nave e saiu
 *  da tela (possivelmente sem ser destruído).
 */
bool inimigoSaiu(Inimigo *foe);

#endif
