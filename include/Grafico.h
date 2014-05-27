/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida de funções relacionadas à parte gráfica (OpenGL).         |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef GRAFICO_H
#define GRAFICO_H

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Tempo de espera para criar um inimigo */
#define TEMPO_INIMIGOS 10 

/* OpenGL */
#define DIST_CAMERA 175 /* Distância da câmera até a nave */
#define SLICES 15
#define STACKS 10

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

void display();
void reshape(int width, int height);

void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

#endif
