#include <stdio.h>   /* sprintf */
#include <string.h>  /* strcmp */
#include "Grafico.h"
#include "Cenario.h"
#include "Textura.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

static void fundo();
static void rio(GLuint tick);
static void parede(GLuint tick);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

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
        (estaEmPrimeiraPessoa()) ? 1.0 - (double) nave.invencibilidade/INVENCIVEL_VIDA : 1.0,
        (estaEmPrimeiraPessoa()) ? 1.0 - (double) nave.invencibilidade/INVENCIVEL_VIDA : 1.0 };

    glLightfv(LUZ_AMBIENTE, GL_SPECULAR, luzTela);
    glLightfv(LUZ_AMBIENTE, GL_AMBIENT,  luzTela);
    glLightfv(LUZ_AMBIENTE, GL_DIFFUSE,  luzTela);

    /* Configura a posição da câmera.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    if (estaEmPrimeiraPessoa()) {
        gluLookAt(nave.base.x, nave.base.y, nave.base.z,
                  0.0, Y_MAX/2, nave.base.z + Z_MAX,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(0.0, Y_MAX/2, nave.base.z - DIST_CAMERA,
                  0.0, Y_MAX/2, nave.base.z + Z_MAX,
                  0.0, 1.0, 0.0);
    }

    /* Elementos estáticos do cenário, com texturas */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    fundo();
    rio(tick);
    parede(tick);
    glDisable(GL_TEXTURE_2D);

    /* Elementos dinâmicos do jogo, ainda sem texturas */    
    for (Celula *p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        desenhaInimigo(foe);
    }
    for (Celula *p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        desenhaProjetil(bullet);
    }
    desenhaNave();
    glDisable(GL_DEPTH_TEST);

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

    /* Espelha horizontalmente a imagem no eixo x */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glScaled(-1.0, 1.0, 1.0);

    /* (ângulo de visão, proporção de tela, distâncias min e max) */
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, Z_MAX + DIST_CAMERA);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

void hud()
{
    const GLdouble RAIO = 5.0;

    glPushMatrix();

    /* Posiciona hud no local apropriado */
    if (estaEmPrimeiraPessoa()) {
        glTranslated(nave.base.x - 6, nave.base.y + 4, nave.base.z + 5);
        glScaled(0.05, 0.05, 0.05);
    } else {
        glTranslated(-GLUT_SCREEN_WIDTH/1.25, 
                      GLUT_SCREEN_HEIGHT/1.5, nave.base.z);
    }

    /* Desenha vidas restantes da nave */
    for (GLint i = 0; i < nave.vidas; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            glColor(CYAN);
            glVertex3d( 0.0  + 3*RAIO*i,   0.0, 0.0);
            glColor(WHITE);
            glVertex3d( 0.0  + 3*RAIO*i,  RAIO, 0.0);
            glVertex3d( RAIO + 3*RAIO*i,   0.0, 0.0);
            glColor(CYAN);
            glVertex3d( 0.0  + 3*RAIO*i, -RAIO, 0.0);
            glVertex3d(-RAIO + 3*RAIO*i,   0.0, 0.0);
            glColor(WHITE);
            glVertex3d( 0.0  + 3*RAIO*i,  RAIO, 0.0);
        } glEnd();
    }

    /* Desenha a caixa da lifebar */
    glColor(DARK_BLUE);
    glRectd(-1.0, -2*RAIO - 2,
            NAVE_HPMAX + 1, -2*RAIO + 1);

    /* Desenha a lifebar, que varia de cor dependendo da energia da nave */
    glColor3ub(1 - 255*nave.base.hp/NAVE_HPMAX, 255*nave.base.hp/NAVE_HPMAX, 0);
    glRectd(0.0, -2*RAIO - 1,
            NAVE_HPMAX*nave.base.hp/100.0, -2*RAIO);

    /* Imprime pontuação */
    char score[16];
    sprintf(score, "Score: %d ", nave.score);
    glColor(WHITE);
    glRasterPos3d(0.0, -2*RAIO - 10, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) score);

    /* Informa se jogo está pausado */
    if (estaPausado()) {
        glColor(WHITE);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, PAUSA_MENSAGEM);
    }

    glPopMatrix();
    tempo();
}

/*------------------------------------------------------------------*/

void fps(GLuint tempo, GLuint tick)
{
    const GLdouble RAIO = 5.0;
    static int tempoAnt = 60;

    glPushMatrix();

    /* Posiciona no local apropriado */
    if (estaEmPrimeiraPessoa()) {
        glTranslated(nave.base.x + 6, nave.base.y + 4, nave.base.z + 5);
        glScaled(0.05, 0.05, 0.05);
    } else {
        glTranslated(+GLUT_SCREEN_WIDTH/1.25, 
                      GLUT_SCREEN_HEIGHT/1.5, nave.base.z);
    }

    /* Imprime pontuação */
    char mostrador[16];
    sprintf(mostrador, "%.2f fps", (double) 1000/tempoAnt);
    glColor(YELLOW);
    glRasterPos3d(0.0, -2*RAIO - 10, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) mostrador);

    /* Talvez provisório também. Evita que mude rápido demais */
    if (tick % 15 == 0) tempoAnt = tempo;

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Desenha o plano de fundo, atribuindo-lhe uma textura.
 *
 */
static void fundo()
{
    glPushMatrix();
    glTranslated(0, 0, nave.base.z + Z_MAX);
    glBindTexture(GL_TEXTURE_2D, fundoTextura);

    glBegin(GL_QUADS); {
        glTexCoord2d(0.0, 1.0); glVertex3d(-26*X_MAX, 15*Y_MAX, 0);
        glTexCoord2d(2.0, 1.0); glVertex3d(26*X_MAX, 15*Y_MAX, 0);
        glTexCoord2d(2.0, 0.0); glVertex3d(26*X_MAX, 0, 0);
        glTexCoord2d(0.0, 0.0); glVertex3d(-26*X_MAX, 0, 0);       
    } glEnd();

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

    const double coord[4][2] = {
        { 0, 8 + t/128 }, { 4, 8 + t/128 },
        { 4,     t/128 }, { 0,     t/128 }
    };

    const int vertex[4][3] = {
        {         0, 0, Z_MAX + DIST_CAMERA },
        { 2 * X_MAX, 0, Z_MAX + DIST_CAMERA },
        { 2 * X_MAX, 0,                   0 },
        {         0, 0,                   0 }
    };

    glPushMatrix();
    glTranslated(-X_MAX, 0, nave.base.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, rioTextura);

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
    const GLdouble DIST_PAREDE = X_MAX;
    GLdouble t = tick % 10000;
    
    glPushMatrix();
    glTranslated(0, 0, nave.base.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, paredeTextura);

    const double coords[4][2] = {
        {      t/128, 1 },
        { 16 + t/128, 1 },
        { 16 + t/128, 0 },
        {      t/128, 0 }
    };

    const int verticesFFLCH[4][3] = {
        { -DIST_PAREDE, Y_MAX, 0 },
        { -DIST_PAREDE, Y_MAX, Z_MAX + DIST_CAMERA },
        { -DIST_PAREDE, 0, Z_MAX + DIST_CAMERA },
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
        { DIST_PAREDE, Y_MAX, Z_MAX + DIST_CAMERA },
        { DIST_PAREDE, 0, Z_MAX + DIST_CAMERA },
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
