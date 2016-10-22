/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do cenário visível e de seus elementos, lidando com       |
 |  inicialização, liberação e atualização do elementos do jogo.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Item.h"
#include "Grafico.h"
#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* 
 *  Coordenadas máximas da área de jogo
 *  (Z_MAX refere-se á distância máxima visível)
 */
#define X_MAX  360
#define Y_MAX  200
#define Z_MAX 1600

/* Variação de vy dos projéteis por timestep */
#define ACEL_GRAVIDADE 0.005

/* Variáveis globais de tempo */
extern GLuint dt, t0;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa nave, pontuação e outras listas.
 */
void inicializaCenario(bool godMode);

/*
 *  Faz a chamada das funções de atualização e desenho, além de
 *  controlar o tempo do jogo, evitando que o mesmo rode rápido
 *  demais e pulando frames que o computador não consiga exibir.
 */
void tempo();

/*
 *  Cuida direta ou indiretamente de tudo que compõe o cenário,
 *  sendo responsável por atualizar posições, tratar interações
 *  entre objetos e encerrar o jogo quando cabível.
 */
void atualiza();

/*
 *  Libera toda a memória alocada para as listas
 *  e texturas e encerra execução do jogo.
 */
void encerraJogo();
