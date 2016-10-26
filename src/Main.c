/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
 |                                                                  |
 |  Uso: ./bin/River [-iddqd] [-d]                                  |
 |      -iddqd: ativa godMode, nave fica invencível;                |
 |          -d: modo debug, exibe informações no terminal.          |
 |          -l: desativa buffer de profundidade.                    |
 |                  Para computadores sem placa de vídeo.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* srand */
#include <time.h>    /* time */
#include <string.h>  /* strcmp */

#include "Cenario.h"
#include "Grafico.h"

/*----------------*
 |   M  A  I  N   |
 *----------------*-------------------------------------------------*/

int main(int argc, char **argv)
{
    /* Tratamento de argumentos via linha de comando */
    bool godMode = false, debug = false, noDepth = false;;
    for (int i = 0; i < argc; i++) {
        if      (strcmp(argv[i], "-iddqd") == 0) godMode = true;
        else if (strcmp(argv[i],     "-d") == 0)   debug = true;
        else if (strcmp(argv[i],     "-l") == 0) noDepth = true;
    }

    /* Inicialização dos elementos de jogo */
    srand(time(NULL));
    inicializaJogo(godMode, debug);
    
    /* OpenGL cuidará do resto */
    inicializaGraficos(noDepth);

    /* Campanha: adote este return. Ele é inútil. */
    return EXIT_SUCCESS;
}
