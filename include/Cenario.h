/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do cenário visível e de todos os seus elementos.          |
 |  Lida desde inicialização e liberação de listas até a            |
 |  atualização do elementos do jogo a cada timestep.               |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef CENARIO_H
#define CENARIO_H

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* 
 *  Coordenadas máximas da área de jogo.
 *  (Z_MAX em tese é infinito; o valor refere-se apenas
 *  à distância máxima visível a partir da posição da nave)
 */
#define X_MAX  80
#define Y_MAX  90
#define Z_MAX 400

/* Valor da aceleração da gravidade */
#define ACEL_GRAV 0.5

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa nave e outras listas.
 */
void inicializa();

/*
 *  Atualiza a posição dos elementos, eliminando os que
 *  saírem da tela ou acertarem a nave, e removendo a 
 *  energia correspondente ao dano neste último caso.
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

#endif
