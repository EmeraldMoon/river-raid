/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida de funções relacionadas à parte gráfica (OpenGL).         |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef GRAFICO_H
#define GRAFICO_H

#include <GL/glut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* OpenGL */
#define DIST_CAMERA 175 /* distância da câmera até a nave */
#define SLICES 15
#define STACKS 10

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

void inicializaGraficos();
void display();
void reshape(int width, int height);

#endif
