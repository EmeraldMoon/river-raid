#include <stdio.h>   /* sprintf */
#include <string.h>  /* strcmp */
#include "Grafico.h"
#include "Cenario.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Identificadores de textura */
GLuint fundoTextura;
GLuint rioTextura;
GLuint paredeTextura;

/* Contagem de timesteps */
static GLuint tick = 0;

static void ignoraComentario(FILE *file);
static void erro(FILE *file, const char *filename);

static void hud();
static void fundo();
static void rio();
static void parede();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void desenha()
{
    /* Faz a limpeza dos buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Coloca câmera atrás da nave.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    glLoadIdentity();
    gluLookAt(0.0, Y_MAX/2, nave.base.z - DIST_CAMERA,
              0.0, Y_MAX/2, nave.base.z + Z_MAX,
              0.0, 1.0, 0.0);

    /* Elementos estáticos do cenário, com texturas */
    glEnable(GL_TEXTURE_2D);
    fundo();
    rio();
    parede();    
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
    hud();  
    
    /* Atualiza o cronômetro */
    tick++;

    glutSwapBuffers();
    
    /* TEMPORÀRIO */
    glutTimerFunc(1000/60, desenha,  1);
    glutTimerFunc(1000/60, atualiza, 1);
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

void carregaTextura(const char *filename, GLuint *textura)
{
    /* Abre o arquivo */
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("carregaTextura()");
        exit(EXIT_FAILURE);
    }

    /* Faz verificação da chave mágica */
    char aux[3];
    fscanf(file, "%2s", aux);
    ignoraComentario(file);
    if (strcmp(aux, "P6") != 0) erro(file, filename);

    /* Obtém largura e altura */
    GLsizei largura, altura;
    ignoraComentario(file);
    if (fscanf(file, "%d %d", &largura, &altura) != 2) erro(file, filename);

    /* Obtém tamanho do arquivo */
    fseek(file, 0, SEEK_END);
    GLsizei n = ftell(file);
    rewind(file);

    /* Lê dados para uma string */
    GLubyte *dados = mallocSafe(sizeof (GLubyte) * n);
    fread(dados, sizeof (GLubyte), n, file);
    fclose(file);

    /* DOOM: Hell begins here */
    glGenTextures(1, textura);
    glBindTexture(GL_TEXTURE_2D, *textura);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura,
                 0, GL_RGB, GL_UNSIGNED_BYTE, dados); 
    /* Hell ends here */
}

static void ignoraComentario(FILE *file)
{
    while (getc(file) == '\n');
    fseek(file, -1, SEEK_CUR);

    if (getc(file) == '#') {
        while (getc(file) != '\n');
    }
    else fseek(file, -1, SEEK_CUR);
}

static void erro(FILE *file, const char *filename)
{
    fprintf(stderr, "carregaTextura(): "
        "%s não é um arquivo PPM\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
}

/*------------------------------------------------------------------*/

void liberaTexturas()
{
    glDeleteTextures(1, &fundoTextura);
    glDeleteTextures(1, &rioTextura);
    glDeleteTextures(1, &paredeTextura);
}

/*------------------------------------------------------------------*
 *
 *  Desenha num ponto fixo da tela um mostrador de atributos da nave
 *  pertinentes à jogabilidade: vidas restantes, energia e pontuação.
 *
 */
static void hud()
{
    static const GLdouble RAIO = 5.0;

    /* Posiciona hud no local apropriado */
    glPushMatrix();
    glTranslated(-GLUT_SCREEN_WIDTH/1.25, GLUT_SCREEN_HEIGHT/1.5, nave.base.z);

    /* Desenha vidas restantes da nave */
    for (int i = 0; i < nave.vidas; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            glColor(CYAN);
            glVertex3d( 0.0  + 3*RAIO*i,   0.0, 0.0);
            glColor(WHITE);
            glVertex3d( 0.0  + 3*RAIO*i,  RAIO, 0.0);
            glColor(WHITE);
            glVertex3d( RAIO + 3*RAIO*i,   0.0, 0.0);
            glColor(CYAN);
            glVertex3d( 0.0  + 3*RAIO*i, -RAIO, 0.0);
            glColor(CYAN);
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
    sprintf(score, "Score: %d", nave.score);
    glColor(WHITE);
    glRasterPos3d(0.0, -2*RAIO - 10, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) score);

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
static void rio()
{
    unsigned int t = tick % 10000;

    glPushMatrix();
    glTranslated(2 * -X_MAX, 0, nave.base.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, rioTextura);

    glBegin(GL_QUADS); {
        glTexCoord2d(0.0, 16.0 + t/128.);
        glVertex3d(0.0, 0.0, Z_MAX + DIST_CAMERA);
        glTexCoord2d(8.0, 16.0 + t/128.);
        glVertex3d(4*X_MAX, 0.0, Z_MAX + DIST_CAMERA);
        glTexCoord2d(8.0, 0.0  + t/128.);
        glVertex3d(4*X_MAX, 0.0, 0.0);
        glTexCoord2d(0.0, 0.0  + t/128.);
        glVertex3d(0.0, 0.0, 0.0);
    } glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Desenha as paredes que limitam lateralmente o jogo, 
 *  atribuindo-lhes uma textura e também produzindo movimento.
 *
 */
static void parede()
{
    static const double DIST_PAREDE = X_MAX;
    unsigned int t = tick % 10000;
    
    glPushMatrix();
    glTranslated(0, 0, nave.base.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, paredeTextura);

    /* Parede esquerda */
    glBegin(GL_QUADS); {
        glTexCoord2d(0.0  + t/128., 1.0);
        glVertex3d(-DIST_PAREDE, Y_MAX, 0);
        glTexCoord2d(16.0 + t/128., 1.0);
        glVertex3d(-DIST_PAREDE, Y_MAX, Z_MAX + DIST_CAMERA);
        glTexCoord2d(16.0 + t/128., 0.0);
        glVertex3d(-DIST_PAREDE, 0, Z_MAX + DIST_CAMERA);
        glTexCoord2d(0.0  + t/128., 0.0);
        glVertex3d(-DIST_PAREDE, 0, 0);
    } glEnd();

    /* Parede direita */
    glBegin(GL_QUADS); {
        glTexCoord2d(0.0  + t/128., 1.0);
        glVertex3d(DIST_PAREDE, Y_MAX, 0);
        glTexCoord2d(16.0 + t/128., 1.0);
        glVertex3d(DIST_PAREDE, Y_MAX, Z_MAX + DIST_CAMERA);
        glTexCoord2d(16.0 + t/128., 0.0);
        glVertex3d(DIST_PAREDE, 0, Z_MAX + DIST_CAMERA);
        glTexCoord2d(0.0  + t/128., 0.0);
        glVertex3d(DIST_PAREDE, 0, 0);
    } glEnd();

    glPopMatrix();
}
