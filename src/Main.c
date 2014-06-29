/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
 |                                                                  |
 |  Uso: ./bin/River [-iddqd]                                      |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdlib.h>  /* atoi, srand */
#include <time.h>    /* time */
#include <string.h>  /* strcmp */
#include "Cenario.h"
#include "Grafico.h"
#include "Teclado.h"
#include "Textura.h"

/* Dimensões da janela */
#define JANELA_LARGURA 2 * 5 * X_MAX
#define JANELA_ALTURA      5 * Y_MAX

/* Cheats */
#define GOD_MODE "-iddqd"

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    if (argc >= 2 && !strcmp(argv[1], GOD_MODE))
        godMode = true;

    /* Inicialização dos elementos */
    inicializaCenario();
    srand(time(NULL));

    /* Inicializa glut e ativa flags */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(JANELA_LARGURA, JANELA_ALTURA);
    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH)  - JANELA_LARGURA)/2,
        (glutGet(GLUT_SCREEN_HEIGHT) - JANELA_ALTURA)/2);
    glutCreateWindow("River Raid");

    /* Carrega texturas */
    inicializaTexturas();

    /* Inicializa efeitos de transparência */
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Inicializa efeitos de luz */
    glEnable(LUZ_AMBIENTE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    /* Nevoeiro sobre o cenário */
    const float cor[3] = { BLACK };
    
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.0009);
    glFogfv(GL_FOG_COLOR, cor);
    glHint(GL_FOG_HINT, GL_NICEST);

    /* ---- Loop principal ---- */

    glutDisplayFunc(desenha);
    glutReshapeFunc(remodela);

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);

    /* ---- Loop principal ---- */

    /* Cuida do resto do jogo */
    glutMainLoop();

    /* Campanha: adote este return. Ele é inútil. */
    return 0;
}
