#include <stdio.h>   /* sprintf */
#include <math.h>    /* sin */

#include "Grafico.h"
#include "Nave.h"
#include "Cenario.h"
#include "Textura.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Tamanho da tela */
GLsizei larg;
GLsizei alt;

/* Provisório? Quebra de encapsulamento? Hmm */
Nave *nave;

static void fundo();
static void rio(GLuint tick);
static void parede(GLuint tick);

static void ortogonalInicio();
static void ortogonalFim();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void inicializaGraficos()
{
    /* Inicializa glut e ativa flags (em tese deveríamos passar
       argc e argv no glutInit(), mas este hack resolve as coisas). */
    glutInit((malloc(0)), NULL);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

    /* Desenha e centraliza janela de jogo */
    glutInitWindowSize(JANELA_LARGURA, JANELA_ALTURA);
    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH)  - JANELA_LARGURA)/2,
        (glutGet(GLUT_SCREEN_HEIGHT) - JANELA_ALTURA)/2);
    glutCreateWindow("River Raid");

    /* Carrega texturas e modelos */
    carregaTexturas();
    nave = carregaNave(false);
    carregaInimigos();

    /* Ativa efeitos de transparência */
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Ativa efeitos de luz */
    glEnable(LUZ_AMBIENTE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    /* Nevoeiro sobre o cenário */
    const GLint cor[3] = { BLACK };
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.0009f);
    glFogiv(GL_FOG_COLOR, cor);
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
}

/*------------------------------------------------------------------*/

void desenha()
{
    /* Contagem de timesteps */
    static GLuint tick = 0;

    /* Faz a limpeza dos buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Configura a luz ambiente */
    const GLfloat luzTela[3] = { 
        1.0, 
        (estaEmPrimeiraPessoa()) ? 1.0 - (double) 1.5*nave->invencibilidade/INVENCIVEL_VIDA : 1.0,
        (estaEmPrimeiraPessoa()) ? 1.0 - (double) 1.5*nave->invencibilidade/INVENCIVEL_VIDA : 1.0
    };
    glLightfv(LUZ_AMBIENTE, GL_SPECULAR, luzTela);
    glLightfv(LUZ_AMBIENTE, GL_AMBIENT,  luzTela);
    glLightfv(LUZ_AMBIENTE, GL_DIFFUSE,  luzTela);

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
    /* Elementos estáticos do cenário, com texturas */
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    fundo();
    rio(tick);
    parede(tick);
    
    /* Elementos dinâmicos do jogo, ainda sem texturas */
    for (Celula *p = getListaItens(); p->prox != NULL; p = p->prox) {
        Item *item = p->prox->item;
        desenhaItem(item);
    }
    for (Celula *p = getListaInimigos(); p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        desenhaInimigo(foe);
    }
    for (Celula *p = getListaProjeteis(); p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        desenhaProjetil(bullet);
    }
    desenhaNave();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    if (exibindoFPS()) fps(dt, tick);
    hud();

    /* Atualiza o cronômetro */
    tick++;

    glutSwapBuffers();
}

/*------------------------------------------------------------------*/

void remodela(GLsizei largura, GLsizei altura)
{
    /* Define tamanho do retângulo de visão */
    glViewport(0, 0, largura, altura);
    larg = largura;
    alt  = altura;

    /* Espelha horizontalmente a imagem no eixo x */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glScaled(-1.0, 1.0, 1.0);

    /* (ângulo de visão, proporção de tela, distâncias min e max) */
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, 1.5*Z_DIST + DIST_CAMERA);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

void hud()
{
    const GLdouble RAIO = larg/75.0;
    const GLdouble K = CONST_CAMERA(31*alt/32.0); /* Constante de mudança de câmera */
    const GLdouble X = (estaEmPrimeiraPessoa() ? nave->corpo.x + larg/10.0
                                               : larg/10.0);
    const GLdouble Y = (estaEmPrimeiraPessoa() ? nave->corpo.y + K*31*alt/32.0
                                               : 31*alt/32.0);
    const GLdouble Z = (estaEmPrimeiraPessoa() ? nave->corpo.z
                                               : nave->corpo.z - DIST_CAMERA);

    ortogonalInicio();

    /* Desenha vidas restantes da nave */
    for (GLint i = 0; i < nave->vidas; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            glColor(CYAN);
            glVertex3d( X + 3*RAIO*i, Y, Z );
            glColor(WHITE);
            glVertex3d( X + 3*RAIO*i, Y + RAIO, Z );
            glVertex3d( X + RAIO + 3*RAIO*i, Y, Z );
            glColor(CYAN);
            glVertex3d( X + 3*RAIO*i, Y - RAIO, Z );
            glVertex3d( X - RAIO + 3*RAIO*i, Y, Z );
            glColor(WHITE);
            glVertex3d( X + 3*RAIO*i, Y + RAIO, Z );
        } glEnd();
    }

    /* Caixa da lifebar */
    const double vertexLifebox[4][3] = {
        {          X - 1.0, Y - 2*RAIO - 3, Z },
        {          X - 1.0, Y - 2*RAIO + 2, Z },
        { X + 0.2*larg + 1, Y - 2*RAIO + 2, Z },
        { X + 0.2*larg + 1, Y - 2*RAIO - 3, Z }
    };

    glColor(DARK_BLUE);
    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++)
        glVertex3dv(vertexLifebox[i]);
    glEnd();

    /* Desenha a lifebar */
    const double vertexLifebar[4][3] = {
        {                               X, Y - 2*RAIO - 2, Z },
        {                               X, Y - 2*RAIO + 1, Z },
        { X + 0.2*larg*nave->atribs.hp/100.0, Y - 2*RAIO + 1, Z },
        { X + 0.2*larg*nave->atribs.hp/100.0, Y - 2*RAIO - 2, Z }
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
    glColor(WHITE);
    glRasterPos3d(X, Y -2*RAIO - 25, Z);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) score);

    /* Informa se jogo está pausado */
    if (estaPausado()) {
        glColor(WHITE);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, PAUSA_MENSAGEM);
    }

    ortogonalFim();
    if (!estaPausado()) tempo();
}

/*------------------------------------------------------------------*/

void fps(GLuint tempo, GLuint tick)
{
    static int tempoAnt = 60;
    const GLdouble K = CONST_CAMERA(31*alt/32.0); /* Constante de mudança de câmera */
    const GLdouble X = (estaEmPrimeiraPessoa() ? nave->corpo.x + 9*larg/10.0
                                               : 9*larg/10.0);
    const GLdouble Y = (estaEmPrimeiraPessoa() ? nave->corpo.y + K*31*alt/32.0
                                               : 31*alt/32.0);
    const GLdouble Z = (estaEmPrimeiraPessoa() ? nave->corpo.z
                                               : nave->corpo.z - DIST_CAMERA);

    ortogonalInicio();

    /* Imprime fps atual */
    char mostrador[16];
    sprintf(mostrador, "%.2f fps", (double) 1000/tempoAnt);
    glColor(YELLOW);

    glRasterPos3d(X, Y, Z);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) mostrador);

    /* Talvez provisório também. Evita que mude rápido demais */
    if (tick % 15 == 0) tempoAnt = tempo;

    ortogonalFim();
}

/*------------------------------------------------------------------*
 *
 *  Desenha o plano de fundo, atribuindo-lhe uma textura.
 *
 */
static void fundo()
{
    glPushMatrix();
    glTranslated(0.0, 0.0, nave->corpo.z + Z_DIST);
    glBindTexture(GL_TEXTURE_2D, fundoTextura);

    const double coord[4][2] = {
        { 0.0, 1.0 }, { 4.0, 1.0 },
        { 4.0, 0.0 }, { 0.0, 0.0 }
    };

    const int vertex[4][3] = {
        { -35*X_MAX, 20*Y_MAX, 0.0 },
        {  35*X_MAX, 20*Y_MAX, 0.0 },
        {  35*X_MAX,      0.0, 0.0 },
        { -35*X_MAX,      0.0, 0.0 }
    };

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coord[i]);
        glVertex3iv(vertex[i]);
    } 
    glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Desenha o "chão" do cenário, o limite inferior do jogo. Faz o
 *  rio se movimentar, a fim de produzir uma sensação de movimento.
 *
 */
static void rio(GLuint tick)
{
    GLdouble t = tick % 10000;
    const GLdouble DIST_RIO = X_MAX + nave->corpo.raio * sin(ANG_MAX);

    glPushMatrix();
    glTranslated(0.0, 0.0, nave->corpo.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, rioTextura);

    const double coord[4][2] = {
        { 0, 8 + t/128 }, { 4, 8 + t/128 },
        { 4,     t/128 }, { 0,     t/128 }
    };

    const int vertex[4][3] = {
        { -DIST_RIO, 0, Z_DIST + DIST_CAMERA },
        {  DIST_RIO, 0, Z_DIST + DIST_CAMERA },
        {  DIST_RIO, 0,                   0 },
        { -DIST_RIO, 0,                   0 }
    };

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coord[i]);
        glVertex3iv(vertex[i]);
    } 
    glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Desenha as paredes que limitam lateralmente o jogo, 
 *  atribuindo-lhes uma textura e também produzindo movimento.
 *
 */
static void parede(GLuint tick)
{
    const GLdouble DIST_PAREDE = X_MAX + nave->corpo.raio * sin(ANG_MAX);
    GLdouble t = tick % 10000;
    
    glPushMatrix();
    glTranslated(0, 0, nave->corpo.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, paredeTextura);

    const double coords[4][2] = {
        {      t/128, 1 },
        { 16 + t/128, 1 },
        { 16 + t/128, 0 },
        {      t/128, 0 }
    };

    const int verticesFFLCH[4][3] = {
        { -DIST_PAREDE, Y_MAX, 0 },
        { -DIST_PAREDE, Y_MAX, Z_DIST + DIST_CAMERA },
        { -DIST_PAREDE, 0, Z_DIST + DIST_CAMERA },
        { -DIST_PAREDE, 0, 0 }
    };

    /* Parede esquerda */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coords[i]);
        glVertex3iv(verticesFFLCH[i]);
    }
    glEnd();

    const int verticesPOLI[4][3] = {
        { DIST_PAREDE, Y_MAX, 0 },
        { DIST_PAREDE, Y_MAX, Z_DIST + DIST_CAMERA },
        { DIST_PAREDE, 0, Z_DIST + DIST_CAMERA },
        { DIST_PAREDE, 0, 0 }
    };

    /* Parede direita */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coords[i]);
        glVertex3iv(verticesPOLI[i]);
    }
    glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*/

/*
 *  Prepara o OpenGL para desenhar objetos em 2D que ficarão fixos
 *  à tela através da matriz de projeção.
 */
static void ortogonalInicio()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, larg, 0, alt); /* Ajusta ao tamanho da tela */

    /* x cresce para a direita, y cresce para cima */
    glScaled(-1.0, 1.0, 1.0); 

    glMatrixMode(GL_MODELVIEW);
}

/*
 *  Finaliza os desenhos de projeções em 2D pelo openGL.
 */
static void ortogonalFim()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
