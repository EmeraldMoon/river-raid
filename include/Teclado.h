/*------------------------------------------------------------------*
 |                                                                  |
 |  Interpretação de comandos realizados pelo teclado.              |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef TECLADO_H
#define TECLADO_H

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Funções chamadas pelo glut toda vez que uma tecla é
 *  pressionada ou solta, alterando seu registro de estado.
 *
 *  OBS: Inteiros x e y devem estar presentes como parâmetros
 *  pela especificação do glut, porém não são usados.
 */
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecialPressed(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

/*
 *  Verificam quais teclas estão pressionadas no momento
 *  e realizam as devidas ações correspondentes.
 */
void keyOperations();
void keySpecialOperations();

#endif
