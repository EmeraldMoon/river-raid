/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções relacionadas à parte gráfica (OpenGL). Cuida da         |
 |  atualização, desenho e montagem do cenário e seus elementos.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>
#include <stdbool.h>
#include "Teclado.h"
#include "Cores.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Número de quadros desenhados por segundo */
#define FPS 75

/* Distância da câmera à nave */
#define DIST_CAMERA 100 
 
/* Constantes para os poliedros */
#define SLICES 16
#define STACKS 10

/* Mensagem a ser exibida quando jogo está pausado */
#define PAUSA_MENSAGEM (unsigned char *) "(Pausa)"

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Loop principal da parte visual. Cuida do posicionamento
 *  da câmera e chamada de funções de atualização.
 */
void desenha();

/*
 *  Redesenha a área de jogo quando (e enquanto)
 *  janela for redimensionada.
 */
void remodela(GLsizei largura, GLsizei altura);

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  energia, vidas restantes e pontuação.
 */
void hud();

/*
 *  Exibe o número de quadros por segundo que o jogo está desenhando
 *  no momento, caso a opção esteja ativada.
 */
void fps(GLuint tempo, GLuint tick);
