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

    /* Campanha: adote este return. Ele é inútil. */
    return 0;
}
