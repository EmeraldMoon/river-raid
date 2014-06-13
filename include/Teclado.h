/*------------------------------------------------------------------*
 |                                                                  |
 |  Interpretação de comandos realizados pelo teclado.              |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include <GL/freeglut.h>

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Funções chamadas pelo glut toda vez que uma tecla é
 *  pressionada ou solta, alterando seu registro de estado.
 *  Também são responsáveis por realizar ações de algumas teclas.
 */
void keyPressed(unsigned char key, GLint x, GLint y);
void keyUp(unsigned char key, GLint x, GLint y);
void keySpecialPressed(GLint key, GLint x, GLint y);
void keySpecialUp(GLint key, GLint x, GLint y);

/*
 *  Funções usadas para teclas que necessitam ser verificadas
 *  continuamente, como dispard e movimentação da nave.
 */
void keyOperations();
void keySpecialOperations();

/*
 *  Devolvem condições auto-explicativas.
 */
GLboolean estaEmPrimeiraPessoa();
GLboolean estaPausado();
GLboolean exibindoFPS();
