/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
 |                                                                  |
 |  Uso: ./bin/River [-iddqd] [-d]                                  |
 |      -iddqd: ativa godMode, nave fica invencível;                |
 |          -d: modo debug, exibe informações no terminal.          |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* srand */
#include <time.h>    /* time */

#include "Cenario.h"
#include "Grafico.h"

/*----------------*
 |   M  A  I  N   |
 *----------------*-------------------------------------------------*/

int main(int argc, char **argv)
{
    /* Inicialização dos elementos de jogo */
    srand(time(NULL));
    inicializaJogo(argc, argv);
    
    /* Isto cuidará do resto do jogo */
    inicializaGraficos();

    /* Campanha: adote este return. Ele é inútil. */
    return EXIT_SUCCESS;
}
