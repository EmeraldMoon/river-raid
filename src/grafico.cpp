#include <cstdio>   /* sprintf */
#include <cstring>  /* strcmp */
#include <cmath>    /* ceil */

#include "grafico.hpp"
#include "lista.hpp"
#include "nave.hpp"
#include "cenario.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "cores.hpp"

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

void inicializaJogo(int argc, char *argv[])
{
    /* Tratamento de argumentos via linha de comando */
    bool godMode = false, debug = false, noDepth = false;;
    for (int i = 0; i < argc; i++) {
        if      (strcmp(argv[i], "-iddqd") == 0) godMode = true;
        else if (strcmp(argv[i], "-d"    ) == 0)   debug = true;
        else if (strcmp(argv[i], "-l"    ) == 0) noDepth = true;
    }    
    /* Inicializa glut. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | (noDepth ? 0 : GLUT_DEPTH));

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),
                       glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("River Raid");

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
    const GLfloat cor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
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

    /* Carrega modelos e listas */
    carregaCenario(godMode, debug);

    nave = getNave();

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
    /* Faz a limpeza dos buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Configura a luz ambiente.
       No modo 1º pessoa, tela fica vermelha após dano. */
    GLfloat k = estaEmPrimeiraPessoa() *
                (1.5 * nave->invencibilidade/INVENCIVEL_VIDA);
    GLfloat luzTela[4] = {1.0f, 1.0f - k, 1.0f - k, 1.0f};
    glLightfv(LUZ_AMBIENTE, GL_AMBIENT, luzTela);

    /* Configura a posição da câmera.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    if (estaEmPrimeiraPessoa()) {
        gluLookAt(nave->corpo.x, nave->corpo.y, nave->corpo.z,
                  nave->corpo.x, nave->corpo.y, nave->corpo.z + Z_DIST,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(0.0, Y_MAX/2.0, nave->corpo.z - DIST_CAMERA,
                  0.0, Y_MAX/2.0, nave->corpo.z + Z_DIST,
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
    controlaTempo(0);
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

/* Constante de mudança de câmera (não pergunte como isso funciona) */
#define constCamera 1/(-Y_MAX/(31/16.0 * altura) + 1)

static void projecaoInicio();
static void projecaoFim();

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  energia, vidas restantes e pontuação.
 */
static void exibeHud()
{
    GLdouble raio = largura/75.0;
    GLdouble x = 0.1 * largura + (estaEmPrimeiraPessoa() * nave->corpo.x);
    GLdouble y = estaEmPrimeiraPessoa() ? 0.85 * altura + nave->corpo.y
                                        : 0.85 * altura * constCamera;
    GLdouble z = nave->corpo.z - (!estaEmPrimeiraPessoa() * DIST_CAMERA);

    projecaoInicio();

    /* Desenha vidas extras da nave */
    for (int i = 0; i < nave->vidas; i++) {
        glBegin(GL_TRIANGLE_FAN);
        setColor(CYAN);
        glVertex3d(x + i * 3*raio,        y, z);
        setColor(WHITE);
        glVertex3d(x + i * 3*raio, y + raio, z);
        glVertex3d(x + raio * (1 + 3*i),  y, z);
        setColor(CYAN);
        glVertex3d(x + i * 3*raio, y - raio, z);
        glVertex3d(x - raio * (1 - 3*i),  y, z);
        setColor(WHITE);
        glVertex3d(x + i * 3*raio, y + raio, z);
        glEnd();
    }
    /* Caixa da lifebar */
    GLdouble vertexLifebox[4][3] = {
        {               x - 1, y - 2*raio - 3, z },
        {               x - 1, y - 2*raio + 2, z },
        { x + 0.2*largura + 1, y - 2*raio + 2, z },
        { x + 0.2*largura + 1, y - 2*raio - 3, z }
    };
    /* Desenha a cabixa */
    setColor(DARK_BLUE);
    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glVertex3dv(vertexLifebox[i]);
    }
    glEnd();

    /* A lifebar em si */
    int hp = nave->atribs.hp;
    GLdouble vertexLifebar[4][3] = {
        {                               x, y - 2*raio - 2, z },
        {                               x, y - 2*raio + 1, z },
        { x + 0.2*largura * hp/NAVE_HPMAX, y - 2*raio + 1, z },
        { x + 0.2*largura * hp/NAVE_HPMAX, y - 2*raio - 2, z }
    };
    /* Cor varia dependendo da energia da nave */
    double r = (double) hp/NAVE_HPMAX;
    glColor3d(1 - r, r, 0.0);

    /* Desenha a lifebar */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glVertex3dv(vertexLifebar[i]);
    }
    glEnd();

    /* Imprime pontuação (e, se for o caso, mensagem de pausa) */
    unsigned char str[32];
    sprintf((char *) str, "Score: %d %s", nave->score,
                                          estaPausado() ? "(pausa)" : "");
    setColor(WHITE);
    glRasterPos3d(x, y - 2*raio - 25, z);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, str);

    projecaoFim();
}

/*
 *  Exibe o número de quadros por segundo que o jogo está
 *  desenhando no momento, caso a opção esteja ativada.
 */
static void exibeFps()
{
    static int fps, cont = 20;

    GLdouble x = 0.88 * largura + (estaEmPrimeiraPessoa() * nave->corpo.x);
    GLdouble y = estaEmPrimeiraPessoa() ? 0.85 * altura + nave->corpo.y
                                        : 0.85 * altura * constCamera;
    GLdouble z = nave->corpo.z - (!estaEmPrimeiraPessoa() * DIST_CAMERA);

    /* FPS só é alterado na tela a cada tantos timesteps */
    cont += getDelayTempo() * FPS/1000.0;
    if (cont >= 20) {
        fps = 1000.0/(getDelayTempo() - 1);
        cont = cont % 20;
    }
    projecaoInicio();

    /* Posiciona projeção */
    glRasterPos3d(x, y, z);

    /* Imprime fps na tela */
    unsigned char mostrador[16];
    sprintf((char *) mostrador, "%2d fps", (fps > 60) ? 60 : fps);
    setColor(YELLOW);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, mostrador);

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
