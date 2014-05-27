/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo que inicializa o programa e chama o loop do OpenGL.      |
 |                                                                  |
 |  Uso: ./bin/River [semente]                                      |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* atoi, srand */
#include <time.h>    /* time */
#include <GL/glut.h>
#include "Cenario.h"
#include "Grafico.h"

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    int semente;

    if (argc < 2) semente = time(NULL);
    else          semente = atoi(argv[1]);

    inicializa();
    srand(semente);

    /* Inicializa */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);

    /* Desenha a janela do jogo */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("River Raid");

    /* ---- Rendering Functions ---- */

    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);

    glutSpecialFunc(keySpecial);
    glutSpecialUpFunc(keySpecialUp);

    /* ---- Rendering Functions ---- */

    glutMainLoop();

    return EXIT_SUCCESS;
}
