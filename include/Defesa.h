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
#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Atributos padrão */
#define FOE_HPMAX  100
#define FOE_RAIO    30
#define FOE_ALTURA 100

/* Pontuação */
#define PONTOS_ACERTO     10
#define PONTOS_DESTRUCT 1000

/*
 *  Representa um inimigo (elemento de defesa).
 */
typedef struct inimigo Inimigo;
struct inimigo
{
    Corpo base;

    /* Chance (esperada) de um projétil disparado acertar a nave */
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
 *  Dispara um projétil em direção à nave e atualiza o tempo
 *  de recarga do inimigo. A fim de aumentar a chance de acerto,
 *  os tiros são mirados logo à frente da posição da nave.
 */
void inimigoDispara(Inimigo *foe);

/*
 *  Verifica se o inimigo passou pela nave e saiu
 *  da tela (possivelmente sem ser destruído).
 */
bool inimigoSaiu(Inimigo *foe);

#endif
