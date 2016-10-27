#include <stdio.h>   /* sprintf */
#include <math.h>    /* ceil */

#include "Grafico.h"
#include "Lista.h"
#include "Nave.h"
#include "Cenario.h"
#include "Teclado.h"
#include "Textura.h"
#include "Cores.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Tipo de luz a ser usada */
#define LUZ_AMBIENTE GL_LIGHT0

/* Tamanho da tela */
static int largura, altura;

/* Ponteiro para acessar a nave neste módulo */
static Nave *nave;

/* Funções perpétuas usadas pelo OpenGL */
static void desenha();
static void remodela(int width, int height);

/* Funções de desenhos fixos na tela */
static void exibeHud();
static void exibeFps();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void inicializaGraficos(GLboolean noDepth)
{
    nave = getNave();
    
    /* Inicializa glut. */
    int temp = 0;
    glutInit(&temp, NULL);  /* evita passar &argc e argv */
    glutInitDisplayMode(GLUT_DOUBLE | (noDepth ? 0 : GLUT_DEPTH));

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),
                       glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("River Raid");

    /* Carrega texturas */
    carregaTexturas();

    /* Ativa efeitos de transparência */
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Ativa efeitos de luz */
    glEnable(LUZ_AMBIENTE);
    glEnable(GL_COLOR_MATERIAL);

    /* Permite desenhar modelos de vértices */
    glEnableClientState(GL_VERTEX_ARRAY);

    /* Nevoeiro sobre o cenário
       (só aceita valores float, infelizmente). */
    const GLfloat cor[3] = {0.0f, 0.0f, 0.0f};
    glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.0008f);
    glFogfv(GL_FOG_COLOR, cor);

    /* Funções perpétuas de desenho */
    glutDisplayFunc(desenha);
    glutReshapeFunc(remodela);

    /* Funções de callback do teclado */
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);

    /* Passa controle do resto do jogo ao OpenGL */
    glutMainLoop();
}

/*------------------------------------------------------------------*/

/*
 *  Loop principal da parte visual. Cuida do posicionamento da câmera,
 *  controle dos buffers e chamada de funções de atualização.
 */
static void desenha()
{
    /* Se jogo estiver pausado, nada é desenhado */
    if (estaPausado()) {
        controlaTempo();
        return;
    }
    /* Faz a limpeza dos buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Configura a luz ambiente.
       No modo 1º pessoa, tela fica vermelha após dano. */
    double k = estaEmPrimeiraPessoa() *
               (1.5 * nave->invencibilidade/INVENCIVEL_VIDA);
    GLfloat luzTela[3] = { 1.0, 1.0 - k, 1.0 - k };
    glLightfv(LUZ_AMBIENTE, GL_AMBIENT, luzTela);

    /* Configura a posição da câmera.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    if (estaEmPrimeiraPessoa()) {
        gluLookAt(nave->corpo.x, nave->corpo.y, nave->corpo.z,
                  nave->corpo.x, nave->corpo.y, nave->corpo.z + Z_DIST,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(0.0, Y_MAX/2, nave->corpo.z - DIST_CAMERA,
                  0.0, Y_MAX/2, nave->corpo.z + Z_DIST,
                  0.0, 1.0, 0.0);
    }
    /* Desenha cenário e elementos de jogo */
    desenhaCenario();

    /* Desenha elementos fixos da tela */
    if (exibindoFPS()) exibeFps();
    exibeHud();

    /* Troca os buffers e pinta a tela */
    glutSwapBuffers();

    /* Passa para o proceesamento não gráfico */
    controlaTempo();
}

/*------------------------------------------------------------------*/

/*
 *  Redesenha a área de jogo quando (e enquanto)
 *  janela for redimensionada.
 */
static void remodela(int width, int height)
{
    /* Variáveis de módulo */
    largura = width;
    altura  = height;

    /* Define tamanho do retângulo de visão */
    glViewport(0, 0, largura, altura);

    /* Espelha horizontalmente a imagem no eixo x */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glScaled(-1.0, 1.0, 1.0);

    /* (ângulo de visão, proporção de tela, distâncias min e max) */
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, Z_DIST);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

static void projecaoInicio();
static void projecaoFim();

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  energia, vidas restantes e pontuação.
 */
static void exibeHud()
{
    const GLdouble RAIO = largura/75.0;
    const GLdouble K = -Y_MAX/(62/32.0 * altura) + 1; /* Constante de mudança de câmera */
    const GLdouble X = (estaEmPrimeiraPessoa() ? nave->corpo.x + largura/10.0
                                               : largura/10.0);
    const GLdouble Y = (estaEmPrimeiraPessoa() ? nave->corpo.y + K*31*altura/32.0
                                               : 31*altura/32.0);
    const GLdouble Z = (estaEmPrimeiraPessoa() ? nave->corpo.z
                                               : nave->corpo.z - DIST_CAMERA);

    projecaoInicio();

    /* Desenha vidas restantes da nave */
    for (GLint i = 0; i < nave->vidas; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            setColor(CYAN);
            glVertex3d( X + 3*RAIO*i, Y, Z );
            setColor(WHITE);
            glVertex3d( X + 3*RAIO*i, Y + RAIO, Z );
            glVertex3d( X + RAIO + 3*RAIO*i, Y, Z );
            setColor(CYAN);
            glVertex3d( X + 3*RAIO*i, Y - RAIO, Z );
            glVertex3d( X - RAIO + 3*RAIO*i, Y, Z );
            setColor(WHITE);
            glVertex3d( X + 3*RAIO*i, Y + RAIO, Z );
        } glEnd();
    }

    /* Caixa da lifebar */
    const double vertexLifebox[4][3] = {
        {          X - 1.0, Y - 2*RAIO - 3, Z },
        {          X - 1.0, Y - 2*RAIO + 2, Z },
        { X + 0.2*largura + 1, Y - 2*RAIO + 2, Z },
        { X + 0.2*largura + 1, Y - 2*RAIO - 3, Z }
    };

    setColor(DARK_BLUE);
    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++)
        glVertex3dv(vertexLifebox[i]);
    glEnd();

    /* Desenha a lifebar */
    const double vertexLifebar[4][3] = {
        {                               X, Y - 2*RAIO - 2, Z },
        {                               X, Y - 2*RAIO + 1, Z },
        { X + 0.2*largura*nave->atribs.hp/100.0, Y - 2*RAIO + 1, Z },
        { X + 0.2*largura*nave->atribs.hp/100.0, Y - 2*RAIO - 2, Z }
    };

    /* Cor varia dependendo da energia da nave */
    glColor3ub(1 - 255*nave->atribs.hp/NAVE_HPMAX, 255*nave->atribs.hp/NAVE_HPMAX, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
        glVertex3dv(vertexLifebar[i]);
    glEnd();

    /* Imprime pontuação */
    char score[16];
    sprintf(score, "Score: %d ", nave->score);
    setColor(WHITE);
    glRasterPos3d(X, Y -2*RAIO - 25, Z);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) score);

    /* Informa se jogo está pausado */
    if (estaPausado()) {
        setColor(WHITE);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) "(Pausa)");
    }

    projecaoFim();
}

/*
 *  Exibe o número de quadros por segundo que o jogo está
 *  desenhando no momento, caso a opção esteja ativada.
 */
static void exibeFps()
{
    static int fps;
    static int cont = 20;

    /* FPS só é alterado na tela a cada tantos timesteps */
    cont += getDelayTempo() * FPS/1000.0;
    if (cont >= 20) {
        fps = 1000.0/(getDelayTempo() - 1);
        cont = cont % 20;
    }
    const GLdouble K = -Y_MAX/(62/32.0 * altura) + 1; /* Constante de mudança de câmera */
    const GLdouble X = (estaEmPrimeiraPessoa() ? nave->corpo.x + 0.88 * largura
                                               : 0.88 * largura);
    const GLdouble Y = (estaEmPrimeiraPessoa() ? nave->corpo.y + K*31*altura/32.0
                                               : 31*altura/32.0);
    const GLdouble Z = (estaEmPrimeiraPessoa() ? nave->corpo.z
                                               : nave->corpo.z - DIST_CAMERA);

    projecaoInicio();

    /* Posiciona projeção */
    glRasterPos3d(X, Y, Z);

    /* String que guarda fps */
    char mostrador[16];
    sprintf(mostrador, "%2d fps", (fps > 60) ? 60 : fps);

    setColor(YELLOW);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) mostrador);

    projecaoFim();
}

/*
 *  Prepara o OpenGL para desenhar objetos em 2D que
 *  ficarão fixos à tela através da matriz de projeção.
 */
static void projecaoInicio()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    /* Ajusta ao tamanho da tela */ 
    gluOrtho2D(0.0, largura, 0.0, altura);

    /* x cresce para a direita, y cresce para cima */
    glScaled(-1.0, 1.0, 1.0); 

    glMatrixMode(GL_MODELVIEW);
}

/*
 *  Finaliza os desenhos de projeções em 2D pelo openGL.
 */
static void projecaoFim()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
