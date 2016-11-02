/*------------------------------------------------------------------*
 |                                                                  |
 |  Interface para trabalhar com projéteis, seja dos inimigos ou    |
 |  do próprio jogador. Lida desde a verificação de posição (saiu   |
 |  da tela, acertou um alvo, etc), até o cálculo da trajetória,    |
 |  baseado na precisão do inimigo e localização do alvo.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <cstdbool>

#include "base.hpp"
#include "lista.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

#define BALA_VEL   9  /* módulo da velocidade */
#define BALA_DANO 15
#define BALA_RAIO  3

/* Desvio-padrão de mira caso precisão == 0 */
#define DESVIO_MAX PI/4

/*------------------------------------------------------------------*/

/*
 *  Representa um projétil causador de dano.
 */
typedef struct projetil Projetil;
struct projetil
{
    /* Corpo esférico */
    Corpo corpo;

    /* Componentes de velocidade do tiro */
    double vx, vy, vz;

    /* Energia removida do elemento em caso de acerto */
    int dano;

    /* Se 'true', tiro foi disparado pela nave */
    bool amigo;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Inicializa lista de projéteis (vazia).
 */
void carregaProjeteis();

/*
 *  Insere um projétil na respectiva lista.
 *  Devolve um ponteiro para o mesmo.
 */
Projetil *criaProjetil();

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
 *  Recebe um projétil. Se colisão for verificada com algum elemento,
 *  remove energia deste e, caso seja um inimigo, verifica também
 *  se o mesmo foi destruído. Caso tiro tenho sido dado pela nave,
 *  são dados pontos ao jogador por acerto e por destruição.
 *
 *  A função devolve true caso haja um acerto qualquer, ou false caso contrário.
 */
bool verificaAcerto(Projetil *bullet);

/*
 *  Recebe um projétil e o desenha na tela.
 */
void desenhaProjetil(Projetil *bullet);

/*
 *  Devolve ponteiro para lista de projéteis.
 */
Lista *getListaProjeteis();

/*
 *  Libera memória alocada para os projéteis.
 */
void liberaProjeteis();
