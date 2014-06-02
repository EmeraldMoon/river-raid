#include <stdio.h>   /* sprintf */
#include <stdlib.h>  /* EXIT_FAILURE */
#include <string.h>  /* strcmp */
#include "Grafico.h"
#include "Cores.h"
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Cenario.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

GLuint texture; /* Imagem do plano de fundo */

static void hud();
static void ground();
static void fundo();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void desenha()
{
    Celula *p;

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    /* Permite o uso de texturas 2D */
    glEnable(GL_TEXTURE_2D);

    /* Coloca câmera atrás da nave.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    gluLookAt(0.0, Y_MAX/2, nave.base.z - DIST_CAMERA,
              0.0, Y_MAX/2, nave.base.z + Z_MAX,
              0.0, 1.0, 0.0);

    /* É necessário que nave seja desenhada depois dos outros
       elementos, pois senão inimigos aparecerão na sua frente */
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        desenhaInimigo(foe);
    }
    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        desenhaProjetil(bullet);
    }
    desenhaNave();
    hud();
    /*fundo();*/
    /* ground(); */    

    glutSwapBuffers();
    glutPostRedisplay(); /* solução temporária */
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
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, Z_MAX);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

static void ignoraComentario(FILE *file);
static void leValor(FILE *file, void *valor);

GLuint carregaTextura(const char * filename)
{
    GLubyte * dados;
    char aux[2];
    GLsizei largura, altura, n;
    FILE * file;

    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("carregaTextura()");
        exit(EXIT_FAILURE);
    }
    ignoraComentario(file);
    fscanf(file, "%2s", aux);
    if (strcmp(aux, "P6") != 0) {
        fprintf(stderr, "carregaTextura(): Não é um arquivo PPM\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    leValor(file, &largura);
    leValor(file, &altura);

    n = largura * altura * 3;
    dados = mallocSafe(sizeof (GLubyte) * n);
    for (int i = 0; i < n; i++) {
        leValor(file, &dados[i]);
    }    

    /* DOOM: Hell begins here */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura,
                 0, GL_RGB, GL_UNSIGNED_BYTE, dados);
    free(dados); 
    /* Hell ends here */

    return texture; 
}

static void ignoraComentario(FILE *file)
{
    if (fgetc(file) == '#') {
        while (fgetc(file) != '\n');
    }
    else fseek(file, -1, SEEK_CUR);
}

static void leValor(FILE *file, void *valorptr)
{
    int *valor = valorptr;
    ignoraComentario(file);
    fscanf(file, "%d", valor);
    valorptr = valor;
}

/*------------------------------------------------------------------*/

void liberaTextura()
{
    glDeleteTextures(1, &texture);
}

/*------------------------------------------------------------------*
 *
 *  Faz a representação visual dos atributos da nave pertinentes à
 *  jogabilidade: vidas restantes, energia e pontuação.
 *
 */
static void hud()
{
    static const double RAIO = 5.0;

    int i;
    char score[16];

    /* Posiciona hud no local apropriado */
    glPushMatrix();
    glTranslated(-150, 100, nave.base.z); /* PROVISÓRIO */

    /* Desenha vidas restantes da nave */
    for (i = 0; i < nave.vidas; i++) {
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

    /* Desenha a lifebar, que varia de verde a vermelho, dependendo do hp da nave */
    glColor3ub(1 - 255*nave.base.hp/NAVE_HPMAX, 255*nave.base.hp/NAVE_HPMAX, 0);
    glRectd(0.0, -2*RAIO - 1,
            NAVE_HPMAX*nave.base.hp/100.0, -2*RAIO);

    /* Imprime pontuação */
    sprintf(score, "Score: %d", nave.score);
    glColor(WHITE);
    glRasterPos3d(0.0, -2*RAIO - 10, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) score);

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Desenha a representação visual do chão, ou seja, o plano que
 *  representa o menor valor que y pode assumir no jogo.
 *
 */
static void ground() {
    glPushMatrix();
    glTranslated(0.0, -Y_MAX/2, nave.base.z - DIST_CAMERA);

    /* Desenha o chão com alguns tons diferentes de azul */
    glBegin(GL_QUADS); {
        glColor(LIGHT_BLUE);
        glVertex3d(2 * -X_MAX, 0.0, 0.0);
        glColor(POWDER_BLUE);
        glVertex3d(2 * X_MAX, 0.0, 0.0);
        glColor(SKY_BLUE);
        glVertex3d(2 * X_MAX, 0.0, Z_MAX + DIST_CAMERA);
        glColor(LIGHT_SKY_BLUE);
        glVertex3d(2 * -X_MAX, 0.0, Z_MAX + DIST_CAMERA);
    } glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *
 *
 */
static void fundo() {
    glPushMatrix();
    glTranslated(-X_MAX, -Y_MAX/2, nave.base.z + Z_MAX-1);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS); {
        glTexCoord2d(0.0,0.0); glVertex3d(0, 0, 0); 
        glTexCoord2d(1.0,0.0); glVertex3d(2*X_MAX, 0, 0); 
        glTexCoord2d(1.0,1.0); glVertex3d(2*X_MAX, Y_MAX, 0);
        glTexCoord2d(0.0,1.0); glVertex3d(0, Y_MAX, 0);
    } glEnd();
}
