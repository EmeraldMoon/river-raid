/*------------------------------------------------------------------*
 |                                                                  |
 |  Interface para trabalhar com projéteis, seja dos inimigos ou    |
 |  do próprio jogador. Lida desde a verificação de posição (saiu   |
 |  da tela, acertou um alvo, etc), até o cálculo da trajetória,    |
 |  baseado na precisão do inimigo e localização do alvo.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef TIRO_H
#define TIRO_H

#include <stdbool.h>
#include "Lista.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

#define BALA_VEL  10  /* módulo da velocidade */
#define BALA_DANO  5  /* dano padrão */
#define BALA_RAIO  3  /* para testes, projéteis esféricos */

/*
 *  Representa um projétil causador de dano.
 */
typedef struct projetil Projetil;
struct projetil
{
    /* Posição do projétil (x,y,z) */
    double x, y, z;

    /* Componentes da velocidade para cada eixo */
    double vx, vy, vz;    

    /* Energia removida do elemento em caso de acerto */
    unsigned int dano;
};

extern Lista *projeteis;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Insere um projétil na respectiva lista.
 */
void criaProjetil(Projetil bullet);

/*  
 *  Aplica no respectivo projétil dois desvios em graus, um horizontal
 *  e outro vertical, ambos inversamente proporcionais à precisão.
 */
void aplicaPrecisao(Projetil *bullet, double precisao);

/*
 *  Atualiza a posição do projétil em relação ao frame anterior.
 */
void moveProjetil(Projetil *bullet);

/*
 *  Recebe um projétil e devolve o tipo de alvo acertado
 *  por ele, ou MISS caso não tenha atingido nenhum.
 */
bool projetilAcertou(Projetil *bullet);

/*
 *  O projétil em questão saiu da tela de jogo?
 *  Considera-se fora de jogo caso saia de um dos limites da tela.
 */
bool projetilSaiu(Projetil *bullet);

#endif
