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

/* Atributos padrão */
#define FOE_HP   100
#define FOE_RAIO  30

/*
 *  Representa um inimigo (elemento de defesa).
 */
typedef struct inimigo Inimigo;
struct inimigo
{
    Esqueleto base;

    /* Probabilidade do projétil acertar a nave */
    double precisao;
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
 *  Dispara um projétil mirando a nave e atualiza o tempo
 *  de recarga, gerando um período de espera até o próximo.
 */
void InimigoDispara(Inimigo *foe);

/*
 *  Verifica se o inimigo passou pela nave e saiu
 *  da tela (possivelmente sem ser destruído).
 */
bool inimigoSaiu(Inimigo *foe);

#endif
