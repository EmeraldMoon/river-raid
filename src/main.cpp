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

#include "grafico.hpp"

/*----------------*
 |   M  A  I  N   |
 *----------------*-------------------------------------------------*/

int main(int argc, char *argv[])
{
    /* OpenGL cuidará do resto */
    inicializaJogo(argc, argv);
}
