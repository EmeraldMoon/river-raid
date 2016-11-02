/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
 |                                                                  |
 |  Uso: ./bin/River [-iddqd] [-d]                                  |
 |      -iddqd: ativa godMode, nave fica invencível;                |
 |          -d: modo debug, exibe informações no terminal.          |
 |          -l: desativa buffer de profundidade.                    |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <cstdlib>  /* srand */
#include <ctime>    /* time */

#include "grafico.hpp"

/*----------------*
 |   M  A  I  N   |
 *----------------*-------------------------------------------------*/

int main(int argc, char *argv[])
{
    /* Inicializa gerador de números pseudoaleatórios */
    srand(time(NULL));
    
    /* OpenGL cuidará do resto */
    inicializaJogo(argc, argv);

    /* Campanha: adote este return. Ele é inútil. */
    return EXIT_SUCCESS;
}
