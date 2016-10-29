/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do cenário visível e de seus elementos, lidando com       |
 |  inicialização, liberação e atualização do elementos do jogo.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include <GL/freeglut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Coordenadas máximas da área de jogo. */
#define X_MAX 360
#define Y_MAX 200

/* Distância máxima visível */
#define Z_DIST 3000

/* Variação negativa de vy dos projéteis por timestep */
#define ACEL_GRAVIDADE 0.005

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Inicializa listas e carrega modelos e texturas para a memória.
 */
void carregaCenario(bool godMode, bool debug);

/*
 *  Faz a chamada das funções de atualização e desenho, além de
 *  controlar o tempo do jogo, evitando que o mesmo rode rápido
 *  demais e pulando frames que o computador não consiga exibir.
 *
 *  É usada a técnica de interpolação: o tempo de jogo é constante,
 *  mesmo que a taxa de desenho na tela passe por variações.
 */
void controlaTempo();

/*
 *  Devolve tempo entre penúltima e última atualizações.
 */
int getDelayTempo();

/*
 *  Cuida direta ou indiretamente de tudo que compõe o cenário,
 *  sendo responsável por atualizar posições, tratar interações
 *  entre objetos e encerrar o jogo quando cabível.
 */
void atualizaCenario();

/*
 *  Desenha todos os elementos do cenário, desde os meramente
 *  visuais (rio, paredes e fundo) até os objetos do jogo
 *  (nave, inimigos, tiros e itens).
 */
void desenhaCenario();

/*
 *  Libera toda a memória alocada para as listas
 *  e texturas e encerra execução do jogo.
 */
void encerraJogo();
