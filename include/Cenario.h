/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do cenário visível e de todos os seus elementos.          |
 |  Lida desde inicialização e liberação de listas até a            |
 |  atualização do elementos do jogo a cada timestep.               |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Coordenadas máximas da área de jogo
   (Z_MAX refere-se á distância máxima visível). */
#define X_MAX  80 
#define Y_MAX  90
#define Z_MAX 800

/* Variação de vy dos projéteis por timestep */
#define ACEL_GRAVIDADE 0.01

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa nave e outras listas.
 */
void inicializaCenario();

/*
 *  Atualiza a posição dos elementos, eliminando os que
 *  saírem da tela ou acertarem a nave, e atualizando
 *  outros valores em decorrência dos acontecimentos.
 */
void atualizaCenario();

/*
 *  Gera um inimigo no fundo do cenário, com altura aleatória
 *  e posição horizontal em um dos quartos extremos da tela.
 *  Inicializa aleatoriamente os atributos básicos do mesmo.
 */
void geraInimigo();

/*
 *  Libera toda a memória alocada para as listas.
 */
void liberaCenario();

/*
 *  Mostra informação a respeito dos elementos do jogo no
 *  timestep atual. Usada para depuração.
 */
void imprimeElementos();
