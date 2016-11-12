/*------------------------------------------------------------------*
 |                                                                  |
 |  Interpretação de comandos realizados pelo teclado.              |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*------------------------------------------------------------------*/

/*
 *  Funções chamadas pelo glut toda vez que uma tecla é
 *  pressionada ou solta, alterando seu registro de estado.
 *  Também são responsáveis por realizar ações de algumas teclas.
 */
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecialPressed(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

/*
 *  Função usada para ações que necessitam ser verificadas
 *  continuamente, como disparo e movimentação da nave.
 */
void keyOperations();

/*
 *  Devolvem condições auto-explicativas.
 */
bool estaEmPrimeiraPessoa();
bool estaPausado();
bool exibindoFPS();
