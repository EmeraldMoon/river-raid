#include <ctype.h>  /* toupper */
#include "Teclado.h"
#include "Cenario.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO   ' '  /* barra de espaços */
#define TECLA_CAMERA 'C'
#define TECLA_PAUSA  'P'
#define TECLA_FPS    'F'
#define TECLA_SAIDA  'Q'

/* Vetores para reconhecimento de teclado */
static GLboolean keyStates[128]        = {GL_FALSE};
static GLboolean keySpecialStates[128] = {GL_FALSE};

/* Variáveis de indicação */
static GLboolean primeiraPessoa = GL_FALSE;
static GLboolean exibeFPS = GL_FALSE;
static GLboolean pausado = GL_FALSE;

static void pausa();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void keyPressed(unsigned char key, GLint x, GLint y)
{
    keyStates[toupper(key)] = GL_TRUE;

    if (keyStates[TECLA_CAMERA]) primeiraPessoa = !primeiraPessoa;
    if (keyStates[TECLA_FPS])    exibeFPS = !exibeFPS;
    if (keyStates[TECLA_PAUSA])  pausa();
    if (keyStates[TECLA_SAIDA])  encerraJogo();
}

/*------------------------------------------------------------------*/
 
void keyUp(unsigned char key, GLint x, GLint y)
{  
    keyStates[toupper(key)] = GL_FALSE;
}

/*------------------------------------------------------------------*/

void keySpecialPressed(GLint key, GLint x, GLint y)
{
    keySpecialStates[key] = GL_TRUE;
}

/*------------------------------------------------------------------*/

void keySpecialUp(GLint key, GLint x, GLint y)
{
    keySpecialStates[key] = GL_FALSE;
}

/*------------------------------------------------------------------*/

void keyOperations()
{
    if (keyStates[TECLA_TIRO] && (nave.base.espera)-- == 0) {
        naveDispara();
    }
}

/*------------------------------------------------------------------*/

void keySpecialOperations()
{
    if      (keySpecialStates[GLUT_KEY_UP])    nave.angVert  += ANG_MANUAL;
    else if (keySpecialStates[GLUT_KEY_DOWN])  nave.angVert  -= ANG_MANUAL;
    if      (keySpecialStates[GLUT_KEY_LEFT])  nave.angHoriz -= ANG_MANUAL;
    else if (keySpecialStates[GLUT_KEY_RIGHT]) nave.angHoriz += ANG_MANUAL;

    /* Ângulos devem permanecer no intervalo [-ANG_MAX, ANG_MAX] */
    if      (nave.angVert  >  ANG_MAX) nave.angVert  =  ANG_MAX;
    else if (nave.angVert  < -ANG_MAX) nave.angVert  = -ANG_MAX;
    if      (nave.angHoriz >  ANG_MAX) nave.angHoriz =  ANG_MAX;
    else if (nave.angHoriz < -ANG_MAX) nave.angHoriz = -ANG_MAX;
}

/*------------------------------------------------------------------*/

GLboolean estaEmPrimeiraPessoa()
{
    return primeiraPessoa;
}

/*------------------------------------------------------------------*/

GLboolean estaPausado()
{
    return pausado;
}

/*------------------------------------------------------------------*/

GLboolean exibindoFPS()
{
    return exibeFPS;
}

/*------------------------------------------------------------------*
 *
 *  Altera o estado atual do jogo, interrompe ou resumindo execução
 *  e, caso cabível, exibindo uma mensagem de pausa na tela.
 *
 */
static void pausa()
{
    pausado = !pausado;

    if (pausado) {
        hud();
        glutSwapBuffers();
        glutIdleFunc(NULL);
    }
    else glutIdleFunc(atualiza);
}
