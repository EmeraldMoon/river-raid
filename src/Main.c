/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
 |                                                                  |
 |  Uso: ./bin/River [-iddqd]                                       |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* srand */
#include <time.h>    /* time */
#include <string.h>  /* strcmp */

#include "Cenario.h"
#include "Grafico.h"

/* Cheats */
#define GOD_MODE "-iddqd"

/*----------------*
 |   M  A  I  N   |
 *----------------*-------------------------------------------------*/

int main(int argc, char **argv)
{
    bool godMode = false;
    if (argc >= 2 && strcmp(argv[1], GOD_MODE) == 0) {
        godMode = true;
    }
    /* Inicialização dos elementos de jogo */
    inicializaCenario(godMode);
    srand(time(NULL));

    /* Isto cuidará do resto do jogo */
    inicializaGraficos();

    /* Campanha: adote este return. Ele é inútil. */
    return EXIT_SUCCESS;
}
