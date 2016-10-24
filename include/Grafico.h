/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções relacionadas à parte gráfica (OpenGL). Cuida da         |
 |  atualização, desenho e montagem do cenário e seus elementos.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include <GL/freeglut.h>

#include "Nave.h"
#include "Cores.h"
#include "Teclado.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Dimensões da janela */
#define JANELA_LARGURA 2 * 5 * X_MAX
#define JANELA_ALTURA      5 * Y_MAX

/* Número de quadros desenhados por segundo */
#define FPS 60

/* Distância da câmera à nave */
#define DIST_CAMERA 250

/* 
 *  Constante para mudança de projeções ortogonais
 *  de terceira para primeira pessoa
 */
#define CONST_CAMERA(k) (-Y_MAX/(2.0*k) + 1)

/* Constantes para os poliedros */
#define SLICES 16
#define STACKS 10

/* Tipos de luz */
#define LUZ_AMBIENTE GL_LIGHT0

/* Mensagem a ser exibida quando jogo está pausado */
#define PAUSA_MENSAGEM (unsigned char *) "(Pausa)"

extern Nave *nave;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Inicializa objetos da parte gráfica e passa o comando
 *  do restante do programa ao OpenGL.
 */
void inicializaGraficos();

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
