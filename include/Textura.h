/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do carregamento e manuseio de texturas da parte gráfica.  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>
#include <stdbool.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Identificadores das texturas */
extern GLuint fundoTextura;
extern GLuint rioTextura;
extern GLuint paredeTextura;
extern GLuint naveTextura;
extern GLuint defesaTextura;
extern GLuint itemTextura;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Faz o carregamento de todas as texturas do jogo.
 */
void carregaTexturas();

/*
 *  Recebe um arquivo em formato PPM contendo uma textura e
 *  um identificador para a mesma. Carrega-a para a memória
 *  a fim de ser usada posteriormente.
 */
void carregaTextura(const char *filename, GLuint *textura, bool mipmap);

/*
 *  Libera a memória alocada por todas as texturas usadas.
 */
void liberaTexturas();
