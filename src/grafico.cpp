#include <cstdio>   /* sprintf */
#include <cstring>  /* strcmp */
#include <cmath>    /* ceil */
#include <memory>
#include <SFML/Graphics.hpp>

#include "grafico.hpp"
#include "nave.hpp"
#include "cenario.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Tamanho da tela */
static int largura, altura;

/* Ponteiro (infelizmente) para acessar neste módulo */
static Nave *nave;

/* Funções perpétuas usadas pelo OpenGL */
static void desenha(sf::Window &janela);
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
    bool godMode = false, debug = false, noDepth = false;
    for (int i = 0; i < argc; i++) {
        if      (strcmp(argv[i], "-iddqd") == 0) godMode = true;
        else if (strcmp(argv[i],     "-d") == 0)   debug = true;
        else if (strcmp(argv[i],     "-l") == 0) noDepth = true;
    }    
    /* Desenha e centraliza janela de jogo */
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow janela(sf::VideoMode(1280, 720),
                      *(new sf::String("River Raid")));
    janela.setFramerateLimit(60);

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
    glFogf(GL_FOG_DENSITY, 0.0001f);
    glFogfv(GL_FOG_COLOR, cor);

    /* static evita que instância seja destruída */
    static Cenario cenario(godMode, debug);

    nave = &cenario.nave;

    // glewExperimental = GL_TRUE;
    // glewInit();

    /* Passa controle do resto do jogo ao OpenGL */
    remodela(1280, 720);
    for (;;)desenha(janela);
    
}

/*------------------------------------------------------------------*/

/*
 *  Loop principal da parte visual. Cuida do posicionamento da câmera,
 *  controle dos buffers e chamada de funções de atualização.
 */
static void desenha(sf::Window &janela)
{
    /* Faz a limpeza dos buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Configura a posição da câmera.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    if (estaEmPrimeiraPessoa()) {
        gluLookAt(nave->getX(), nave->getY(), nave->getZ(),
                  nave->getX(), nave->getY(), nave->getZ() + Cenario::Z_DIST,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(0.0, Cenario::Y_MAX/2.0, nave->getZ() - DIST_CAMERA,
                  0.0, Cenario::Y_MAX/2.0, nave->getZ() + Cenario::Z_DIST,
                  0.0, 1.0, 0.0);
    }
    /* Desenha cenário e elementos de jogo */
    Cenario::get().desenha();

    /* Desenha elementos fixos da tela */
    // if (exibindoFPS()) exibeFps();
    // exibeHud();

    janela.display();

    /* Passa para o proceesamento não gráfico */
    Cenario::get().atualiza();
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
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, Cenario::Z_DIST);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

/* Constante de mudança de câmera (não pergunte como isso funciona) */
#define constCamera 1/(-Cenario::Y_MAX/(31/16.0 * altura) + 1)

static void projecaoInicio();
static void projecaoFim();

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  energia, vidas restantes e pontuação.
 */
static void exibeHud()
{
    GLdouble raio = largura/75.0;
    GLdouble x = 0.1 * largura + (estaEmPrimeiraPessoa() * nave->getX());
    GLdouble y = estaEmPrimeiraPessoa() ? 0.85 * altura + nave->getY()
                                        : 0.85 * altura * constCamera;
    GLdouble z = nave->getZ() - ((not estaEmPrimeiraPessoa()) * DIST_CAMERA);

    projecaoInicio();

    /* Desenha vidas extras da nave */
    for (int i = 0; i < nave->getVidas(); i++) {
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
    double r = (double) nave->getHP()/nave->getHPMax();
    GLdouble vertexLifebar[4][3] = {
        {                   x, y - 2*raio - 2, z },
        {                   x, y - 2*raio + 1, z },
        { x + 0.2*largura * r, y - 2*raio + 1, z },
        { x + 0.2*largura * r, y - 2*raio - 2, z }
    };
    /* Cor varia dependendo da energia da nave */
    glColor3d(1 - r, r, 0.0);

    /* Desenha a lifebar */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glVertex3dv(vertexLifebar[i]);
    }
    glEnd();

    /* Imprime pontuação (e, se for o caso, mensagem de pausa) */
    sf::String str = "Score: " + nave->getScore();
    if (estaPausado()) str += " (pausa)";
    setColor(WHITE);
    glRasterPos3d(x, y - 2*raio - 25, z);

    sf::Font fonte;
    fonte.loadFromFile("DejaVuSans.ttf");
    // sf::Text(str, fonte, 18);

    projecaoFim();
}

/*
 *  Exibe o número de quadros por segundo que o jogo está
 *  desenhando no momento, caso a opção esteja ativada.
 */
static void exibeFps()
{
    static int fps, cont = 20;

    GLdouble x = 0.88 * largura + (estaEmPrimeiraPessoa() * nave->getX());
    GLdouble y = estaEmPrimeiraPessoa() ? 0.85 * altura + nave->getY()
                                        : 0.85 * altura * constCamera;
    GLdouble z = nave->getZ() - ((not estaEmPrimeiraPessoa()) * DIST_CAMERA);

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
    sf::String mostrador = (fps > 60 ? 60 : fps) + " fps";
    setColor(YELLOW);
    
    sf::Font fonte;
    fonte.loadFromFile("DejaVuSans.ttf");
    // sf::Text(mostrador, fonte, 18);

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
