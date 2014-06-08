/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo principal do projeto.                                    |
 |  Inicializa o programa e chama o loop do OpenGL.                 |
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

/* Intervalo de milisegundos para chamar tempo() */
#define MILISEG 5

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    int semente;

    if (argc < 2) semente = time(NULL);
    else          semente = atoi(argv[1]);

    /* Inicialização dos elementos */
    inicializaCenario();
    srand(semente);

    /* Inicializa glut e ativa flags */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);

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

    /* Inicializa efeitos de luz (com alguns problemas) */
    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(LUZ_AMBIENTE);  
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    */

    /* ---- Loop principal ---- */

    /*glutIdleFunc(atualiza);*/

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
