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

/* Taxa de atualização padrão */
#define FPS 60

/* Distância da câmera à nave */
#define DIST_CAMERA 100 
 
/* Constantes para os poliedros */
#define SLICES 16
#define STACKS 10

/* Mensagem a ser exibida quando jogo está pausado */
#define PAUSA_MENSAGEM (unsigned char *) "(Pausa)"

/* Identificadores das texturas */
extern GLuint fundoTextura;
extern GLuint rioTextura;
extern GLuint paredeTextura;

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
 *  Recebe um arquivo em formato PPM contendo uma textura e
 *  um identificador para a mesma. Carrega-a para a memória
 *  a fim de ser usada posteriormente.
 */
void carregaTextura(const char *filename, GLuint *textura);

/*
 *  Libera a memória alocada por todas as variáveis texture.
 */
void liberaTexturas();

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  energia, vidas restantes e pontuação.
 */
void hud();
