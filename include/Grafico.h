/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções relacionadas à parte gráfica (OpenGL).                  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Dimensões da janela (widescreen por padrão) */
#define JANELA_LARGURA 3 * X_MAX
#define JANELA_ALTURA  JANELA_LARGURA/16.0 * 9

/* Número de quadros desenhados por segundo */
#define FPS 60

/* Distância da câmera à nave */
#define DIST_CAMERA 250

/* Constantes para os poliedros */
#define SLICES 16
#define STACKS 10

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Inicializa elementos do glut e OpenGL e passa
 *  o comando do restante do programa ao OpenGL.
 *  noDepth indica se deveremos usar o buffer de profundidade.
 */
void inicializaGraficos(GLboolean noDepth);

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
