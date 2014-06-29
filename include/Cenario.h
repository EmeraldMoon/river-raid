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
#include "Grafico.h"
#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Coordenadas máximas da área de jogo
   (Z_MAX refere-se á distância máxima visível). */
#define X_MAX  120
#define Y_MAX  100
#define Z_MAX 1600

/* Variação de vy dos projéteis por timestep */
#define ACEL_GRAVIDADE 0.005

/* SOLUÇÂO TEMPORÀRIA ATÉ PENSAR NUMA IDEIA MELHOR.
   SIM, EU GOSTO DE CAPS LOCK */
GLuint dt;

/* Cheat da nave indestrutível */
extern bool godMode;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa nave, pontuação e outras listas.
 */
void inicializaCenario();

void tempo();

/*
 *  Loop principal de processamento do jogo. Cuida direta
 *  ou indiretamente de tudo que compõe o cenário, sendo
 *  responsável por atualizar posições, tratar interações
 *  entre objetos e encerrar o jogo quando cabível.
 */
void atualiza();

/*
 *  Libera toda a memória alocada para as listas
 *  e texturas e encerra execução do jogo.
 */
void encerraJogo();
