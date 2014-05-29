#include <ctype.h>  /* toupper */
#include <stdbool.h>
#include <GL/glut.h>
#include "Grafico.h"
#include "Cenario.h"
#include "Nave.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO  ' '
#define TECLA_SAIDA 'Q'

/* Vetores para reconhecimento de teclado */
GLboolean keyStates[128]        = {GL_FALSE};
GLboolean keySpecialStates[128] = {GL_FALSE};

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void keyPressed(unsigned char key, int x, int y)
{
    keyStates[toupper(key)] = GL_TRUE;
}

/*------------------------------------------------------------------*/
 
void keyUp(unsigned char key, int x, int y)
{  
    keyStates[toupper(key)] = GL_FALSE;
}

/*------------------------------------------------------------------*/

void keySpecialPressed(int key, int x, int y)
{
    keySpecialStates[key] = GL_TRUE;
}

/*------------------------------------------------------------------*/

void keySpecialUp(int key, int x, int y)
{
    keySpecialStates[key] = GL_FALSE;
}

/*------------------------------------------------------------------*/

void keyOperations()
{
    if (keyStates[TECLA_TIRO]) naveDispara();
    if (keyStates[TECLA_SAIDA]) {
        liberaCenario();
        exit(0);
    }

    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

void keySpecialOperations()
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
