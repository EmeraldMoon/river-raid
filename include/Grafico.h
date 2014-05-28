/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida de funções relacionadas à parte gráfica (OpenGL).         |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef GRAFICO_H
#define GRAFICO_H

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

void centralizaJanela(int width, int height);
void display();
void reshape(int width, int height);

void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

#endif
