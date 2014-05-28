#include <stdio.h>  /* printf, puts */
#include <ctype.h>  /* toupper */
#include <stdbool.h>
#include <GL/glut.h>
#include "Grafico.h"
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Cenario.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Macros para teclas */
#define TECLA_TIRO  ' '
#define TECLA_SAIDA 'Q'

/* Tempo de espera para criar um inimigo */
#define TEMPO_INIMIGOS 10

/* OpenGL */
#define DIST_CAMERA 175 /* Distância da câmera até a nave */
#define SLICES 15
#define STACKS 10

/* Vetores para reconhecimento de teclado */
GLboolean keyStates[128]        = {GL_FALSE};
GLboolean keySpecialStates[128] = {GL_FALSE};

static void keyOperations();
static void keySpecialOperations();

static void desenhaNave();
static void desenhaInimigos();
static void desenhaProjeteis();
static void hud();
static void ground();

static void imprimeElementos();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void centralizaJanela(int width, int height)
{
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)  - width)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - height)/2);
}

/*------------------------------------------------------------------*/

void display()
{
    /* Tempo de espera até gerar um inimigo */
    static int cont = TEMPO_INIMIGOS;

    /* Reconhecimento do teclado */
    keyOperations();
    keySpecialOperations();

    /* Limpa fundo de tela e prepara para desenhar */
    /*glClearColor(0, 0, 0, 255);*/
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    /* Coloca câmera atrás da nave */
    gluLookAt(0.0, Y_MAX/2, nave.base.z - DIST_CAMERA,
              0.0, Y_MAX/2, nave.base.z + Z_MAX,
              0.0, 1.0, 0.0);
    /*glTranslated(0.0, -Y_MAX/2, nave.base.z - DIST_CAMERA);
    glRotated(180.0, 0.0, 1.0, 0.0);*/

    /* Desenhos */
    desenhaNave();
    desenhaInimigos();
    desenhaProjeteis();
    hud();
    /*ground();*/

    /* Atualizações */
    atualizaCenario();
    imprimeElementos(); 
    if (cont-- == 0) {
        geraInimigo();
        cont = TEMPO_INIMIGOS;
    }
    if (nave.vidas <= 0) {
        liberaCenario();
        exit(EXIT_SUCCESS);
    }

    glutSwapBuffers();
}

/*------------------------------------------------------------------*/

void reshape(GLsizei width, GLsizei height)
{ 
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glScaled(-1.0, 1.0, 1.0);
    gluPerspective(60, (GLdouble) width/height, 1.0, Z_MAX);
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

void keyPressed(unsigned char key, int x, int y)
{
    keyStates[toupper(key)] = true;
}

/*------------------------------------------------------------------*/
 
void keyUp(unsigned char key, int x, int y)
{  
    keyStates[toupper(key)] = false;
}

/*------------------------------------------------------------------*/

void keySpecial(int key, int x, int y)
{
    keySpecialStates[key] = true;
}

/*------------------------------------------------------------------*/

void keySpecialUp(int key, int x, int y)
{
    keySpecialStates[key] = false;
}

/*------------------------------------------------------------------*/

static void keyOperations()
{
    if (keyStates[TECLA_TIRO]) naveDispara();
    if (keyStates[TECLA_SAIDA]) {
        liberaCenario();
        exit(0);
    }

    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

static void keySpecialOperations()
{
    if (keySpecialStates[GLUT_KEY_UP])    nave.angVert  += ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_DOWN])  nave.angVert  -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_LEFT])  nave.angHoriz -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_RIGHT]) nave.angHoriz += ANG_MANUAL;

    /* Ângulos devem estar no intervalo [-ANG_MAX, ANG_MAX] */
    if      (nave.angVert  >  ANG_MAX) nave.angVert  =  ANG_MAX;
    else if (nave.angVert  < -ANG_MAX) nave.angVert  = -ANG_MAX;
    if      (nave.angHoriz >  ANG_MAX) nave.angHoriz =  ANG_MAX;
    else if (nave.angHoriz < -ANG_MAX) nave.angHoriz = -ANG_MAX;

    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

static void desenhaNave()
{
    glPushMatrix();
    glTranslated(nave.base.x, nave.base.y, nave.base.z);
    glRotated(nave.angHoriz * 180.0/PI,  0.0, 1.0, 0.0);
    glRotated(nave.angVert  * 180.0/PI, -1.0, 0.0, 0.0);    
    glColor3ub(255, 255, 0); /* Amarelo */
    glutWireCone(nave.base.raio, nave.base.altura + 20, SLICES, STACKS);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

static void desenhaInimigos()
{
    Celula *p;

    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        glPushMatrix();
        glTranslated(foe->base.x, foe->base.y, foe->base.z);
        glRotated(-90.0, 1.0, 0.0, 0.0);
        glColor3ub(255, 0, 0); /* Vermelho */
        glutWireCone(foe->base.raio, foe->base.altura, SLICES, STACKS);
        glPopMatrix();
    }
}

/*------------------------------------------------------------------*/

static void desenhaProjeteis()
{
    Celula *p;

    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        glPushMatrix();
        glTranslated(bullet->x, bullet->y, bullet->z);
        glColor3ub(0, 255, 0); /* Verde */
        glPointSize(20.0);  
        glutSolidSphere(bullet->raio, SLICES, STACKS);  
        glPopMatrix();
    }

}

/*------------------------------------------------------------------*/

static void hud()
{
    int i, tam;
    char score[16];

    glPushMatrix();
    glTranslated(-GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT, nave.base.z);

    /* Imprime vidas restantes da nave */
    glColor3ub(0, 255, 255); /* ciano */
    for (i = 0; i < nave.vidas; i++) {
        glPointSize(50.0);  
        glBegin(GL_TRIANGLE_FAN);
            glVertex3d( 0.0  - 15*i,  0.0, 0.0);
            glVertex3d( 0.0  - 15*i,  5.0, 0.0);
            glVertex3d( 5.0  - 15*i,  0.0, 0.0);
            glVertex3d( 0.0  - 15*i, -5.0, 0.0);
            glVertex3d(-5.0  - 15*i,  0.0, 0.0);
            glVertex3d( 0.0  - 15*i,  5.0, 0.0);
        glEnd();
    }

    /* Imprime hp da nave */
    glColor3ub(0, 255, 0); /* Verde */
    glBegin(GL_LINES); {
        glVertex3d(5.0, -10.0, 0.0);
        glVertex3d(5.0 - NAVE_HPMAX*nave.base.hp/100.0, -10.0, 0.0);
    } glEnd();

    /* Imprime score */
    tam = sprintf(score, "Score: %d", nave.score);
    glColor3d(1.0, 1.0, 1.0);
    glRasterPos3d(5.0, -20.0, 0.0);
    for (i = 0; i < tam; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);
    }

    glPopMatrix();
}

/*------------------------------------------------------------------*/

static void ground() {
    glPushMatrix();
    glTranslated(0.0, -Y_MAX/2, nave.base.z - DIST_CAMERA);

    /* Desenha o chão com alguns tons diferentes de marrom */
    glBegin(GL_QUADS); {
        glColor3d(0.8f, 0.25f, 0.1f);
        glVertex3d(2 * -X_MAX, 0.0, 0.0);
        glColor3d(0.4f, 0.4f, 0.1f);
        glVertex3d(2 * X_MAX, 0.0, 0.0);
        glColor3d(0.8f, 0.25f, 0.1f);
        glVertex3d(2 * X_MAX, 0.0, Z_MAX + DIST_CAMERA);
        glColor3d(0.5f, 0.3f, 0.1f);
        glVertex3d(2 * -X_MAX, 0.0, Z_MAX + DIST_CAMERA);
    } glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*
 *
 *  Mostra informação a respeito de todos os elementos do jogo neste
 *  momento. Para efeitos de clareza, todas as componentes Z, exceto
 *  a da nave, são relativas à nave em si (e não absolutas).
 *
 */
static void imprimeElementos()
{
    Celula *p;

    /* Limpa a tela do terminal/prompt */
    #ifdef __linux__
            system("clear");
    #elif _WIN32
            system("cls");
    #endif

    puts("{Nave}");
    printf("PONTUAÇÂO: %d\n", nave.score);
    printf("VIDAS: %d\n", nave.vidas);
    printf("Energia: %-3d/%d\n", nave.base.hp, NAVE_HPMAX);
    printf("Posição: (%.0f, %.0f, %.0f)\n", 
        nave.base.x, nave.base.y, nave.base.z);
    printf("Ângulos: (%.0f°, %.0f°)\n",
        (180/PI) * nave.angHoriz, (180/PI) * nave.angVert);
    
    puts("\n{Inimigos}");
    puts("   ( x, y, z)       Recarga    Precisão    Energia ");
    puts("----------------    -------    --------   ---------");
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%3g, %3g, %3g)      %2d/%2d       %3.0f%%       %2d/%2d\n",
            foe->base.x, foe->base.y, (foe->base.z - nave.base.z),
            foe->base.espera, foe->base.cooldown, 100 * foe->precisao,
            foe->base.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("   ( x, y, z)          [ vx, vy, vz]        Amigo? ");
    puts("----------------    --------------------   --------");
    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%3.0f, %2.0f, %3.0f)      [%4.1f, %4.1f, %4.1f]       %s\n",
            bullet->x, bullet->y, (bullet->z - nave.base.z),
            bullet->vx, bullet->vy, bullet->vz,
            (bullet->amigo) ? "sim" : "não");
    }
}
