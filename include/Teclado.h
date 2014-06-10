/*------------------------------------------------------------------*
 |                                                                  |
 |  Interpretação de comandos realizados pelo teclado.              |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

extern GLboolean pausado;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Funções chamadas pelo glut toda vez que uma tecla é
 *  pressionada ou solta, alterando seu registro de estado.
 *
 *  OBS: Inteiros x e y correspondem à posição do mouse e
 *  precisam estar presentes, porém não são usados.
 */
void keyPressed(GLubyte key, GLint x, GLint y);
void keyUp(GLubyte key, GLint x, GLint y);
void keySpecialPressed(GLint key, GLint x, GLint y);
void keySpecialUp(GLint key, GLint x, GLint y);

/*
 *  Funções responsáveis por verificar quais teclas estão
 *  pressionadas no momento e realizar as ações correspondentes.
 */
void keyOperations();
void keySpecialOperations();
