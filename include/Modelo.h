/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições de uso geral e estruturas comuns, a serem            |
 |  usadas em mais de um módulo do projeto.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>  /* GLdouble, GLsizei */

/*-------------------------*----------------------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Diretório onde arquivos .vert e .norm estão localizados */
#define MODELO_DIR "model/"

/*-------------------------*----------------------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Recebe uma matriz de doubles e dimensões (num x 3) e um nome
 *  de arquivo. Lê valores de vetores (vértices ou normais) para a matriz.
 */
void leVetores(GLdouble vetores[][3], GLsizei num, char nomeArq[]);
