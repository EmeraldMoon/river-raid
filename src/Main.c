/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do glut.                   |
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

/* Arquivos de textura */
#define FUNDO_TEXTURA  "texture/space.ppm"
#define RIO_TEXTURA    "texture/water.ppm"
#define PAREDE_TEXTURA "texture/brick.ppm"

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    int semente = (argc < 2) ? time(NULL)
                             : atoi(argv[1]);

    /* Inicialização dos elementos */
    inicializaCenario();
    srand(semente);

    /* Inicializa glut e ativa flags */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(JANELA_LARGURA, JANELA_ALTURA);
    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH)  - JANELA_LARGURA)/2,
        (glutGet(GLUT_SCREEN_HEIGHT) - JANELA_ALTURA)/2);
    glutCreateWindow("River Raid");

    /* Carrega texturas */
    carregaTextura(FUNDO_TEXTURA,  &fundoTextura);
    carregaTextura(RIO_TEXTURA,    &rioTextura);
    carregaTextura(PAREDE_TEXTURA, &paredeTextura);

    /* Ativa o uso de transparência */
    glEnable(GL_BLEND);

    /* Inicializa efeitos de luz (com alguns problemas) */
    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(LUZ_AMBIENTE);  
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    */

    /* ---- Loop principal ---- */

    glutDisplayFunc(desenha);
    glutReshapeFunc(remodela);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
