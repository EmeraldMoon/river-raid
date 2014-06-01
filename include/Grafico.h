/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções relacionadas à parte gráfica (OpenGL). Cuida da         |
 |  atualização, desenho e montagem do cenário e seus elementos.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Distância da câmera à nave */
#define DIST_CAMERA 100 

/* Constantes para os poliedros */
#define SLICES 15
#define STACKS 10

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Loop principal da parte visual. Cuida 
 */
void desenha();

/*
 *  Redesenha a área de jogo quando 
 *  (e enquanto) janela for redimensionada. 
 */
void remodela(GLsizei width, GLsizei height);
