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

static int  itoa(char str[25], unsigned int pontos);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    /* Coloca câmera atrás da nave */
    glTranslatef(0.0f, -Y_MAX/2, nave.base.z - DIST_CAMERA);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    /* Desenhos */
    hud();
    desenhaNave();
    desenhaInimigos();
    desenhaProjeteis();

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

void reshape(int width, int height)
{ 
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(60, (GLfloat) width / (GLfloat) height, 1.0, 500.0); 
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
    if (keySpecialStates[GLUT_KEY_UP])    nave.angY += ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_DOWN])  nave.angY -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_RIGHT]) nave.angX -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_LEFT])  nave.angX += ANG_MANUAL;

    /* Ângulos devem estar no intervalo [-ANG_MAX, ANG_MAX] */ 
    if      (nave.angX >  ANG_MAX) nave.angX =  ANG_MAX;
    else if (nave.angX < -ANG_MAX) nave.angX = -ANG_MAX;
    if      (nave.angY >  ANG_MAX) nave.angY =  ANG_MAX;
    else if (nave.angY < -ANG_MAX) nave.angY = -ANG_MAX;

    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

static void desenhaNave()
{
    glPushMatrix();
    glTranslatef(nave.base.x, nave.base.y, nave.base.z);
    glRotatef(nave.angX*180.0/PI, 1.0, 0.0, 0.0);
    glRotatef(nave.angY*180.0/PI, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0); /* Amarelo */
    glutWireCone(NAVE_RAIO, NAVE_ALTURA + 20, SLICES, STACKS);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

static void desenhaInimigos()
{
    Celula *p;

    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        glPushMatrix();
        glTranslatef(foe->base.x, foe->base.y, foe->base.z);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0, 0.0, 0.0); /* Vermelho */
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
        glTranslatef(bullet->x, bullet->y, bullet->z);
        glColor3f(0.0, 1.0, 0.0); /* Verde */
        glPointSize(20.0f);  
        glutSolidSphere(BALA_RAIO, SLICES, STACKS);  
        glPopMatrix();
    }

}

/*------------------------------------------------------------------*/

static void hud()
{
    int i, tam;
    char score[25] = "Score: ";

    glPushMatrix();
    glTranslatef(1.15*X_MAX, 1.5*Y_MAX, nave.base.z);

    /* Imprime vidas restantes da nave */
    glColor3f(0.0, 1.0, 1.0); /* Azul claro */
    for (i = 0; i < nave.vidas; i++) {
        glPointSize(50.0f);  
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f( 0.0f  - 15*i,  0.0f, 0.0f);
            glVertex3f( 0.0f  - 15*i,  5.0f, 0.0f);
            glVertex3f( 5.0f  - 15*i,  0.0f, 0.0f);
            glVertex3f( 0.0f  - 15*i, -5.0f, 0.0f);
            glVertex3f(-5.0f  - 15*i,  0.0f, 0.0f);
            glVertex3f( 0.0f  - 15*i,  5.0f, 0.0f);
        glEnd();
    }

    /* Imprime hp da nave */
    glColor3f(0.0, 1.0, 0.0); /* Verde */
    glBegin(GL_LINES); {
        glVertex3f(5.0f, -10.0f, 0.0f);
        glVertex3f(5.0f - NAVE_HPMAX*nave.base.hp/100.0, -10.0f, 0.0f);
    } glEnd();

    /* Imprime score */
    tam = itoa(score, nave.score);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(5.0f, -20.0f, 0.0f);
    for (i = 0; i < tam; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);

    glPopMatrix();
}

/*------------------------------------------------------------------*/

static int itoa(char str[25], unsigned int pontos)
{
    int i = 7; /* Considera que já existe "Score: " em str */

    if (pontos > 0) {
        i = itoa(str, pontos/10);
        str[i] = pontos % 10 + '0';
        return ++i;
    }

    return i;
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
        (180/PI) * nave.angX, (180/PI) * nave.angY);
    
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
