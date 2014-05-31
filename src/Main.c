/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo que inicializa o programa e chama o loop do OpenGL.      |
 |                                                                  |
 |  Uso: ./bin/River [semente]                                      |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* atoi, srand */
#include <time.h>    /* time */
#include "Cenario.h"
#include "Grafico.h"
#include "Teclado.h"

/* Dimensões da janela */
#define JANELA_LARGURA 2 * 5 * X_MAX
#define JANELA_ALTURA      5 * Y_MAX

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    int semente;

    if (argc < 2) semente = time(NULL);
    else          semente = atoi(argv[1]);

    /* Inicialização de elementos do jogo */
    inicializaCenario();
    srand(semente);

    /* Inicializa glut e cuida de todo o resto */
    glutInit(&argc, argv);
    inicializaGraficos();

    /* Inicializa flags */    
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(JANELA_LARGURA, JANELA_ALTURA);
    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH)  - JANELA_LARGURA)/2,
        (glutGet(GLUT_SCREEN_HEIGHT) - JANELA_ALTURA)/2);
    glutCreateWindow("River Raid");    

    /* ---- Funções de renderização ---- */

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);

    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);

    /* ---- Funções de renderização ---- */

    /* Cuida do resto do jogo */
    glutMainLoop();

    /* Campanha: adote este return. Ele é inútil. */
    return 0;
}
