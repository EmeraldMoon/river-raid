/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Tão vazio quanto o significado de nossa existência.             |
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
    /* OpenGL e inicialização */
    inicializaJogo(argc, argv);
}
