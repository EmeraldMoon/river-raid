#include <ctype.h>  /* toupper */
#include "Teclado.h"
#include "Nave.h"
#include "Cenario.h"
#include "Grafico.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO  ' '  /* barra de espaços */
#define TECLA_SAIDA 'Q'

/* Vetores para reconhecimento de teclado */
GLboolean keyStates[128]        = {GL_FALSE};
GLboolean keySpecialStates[128] = {GL_FALSE};

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void keyPressed(GLubyte key, GLint x, GLint y)
{
    keyStates[toupper(key)] = GL_TRUE;
}

/*------------------------------------------------------------------*/
 
void keyUp(GLubyte key, GLint x, GLint y)
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
    if (keyStates[TECLA_SAIDA]) {
        liberaCenario();
        exit(0);
    }
}

/*------------------------------------------------------------------*/

void keySpecialOperations()
{
    if (keySpecialStates[GLUT_KEY_UP])    nave.angVert  += ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_DOWN])  nave.angVert  -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_LEFT])  nave.angHoriz -= ANG_MANUAL;
    if (keySpecialStates[GLUT_KEY_RIGHT]) nave.angHoriz += ANG_MANUAL;

    /* Ângulos devem permanecer no GLintervalo [-ANG_MAX, ANG_MAX] */
    if      (nave.angVert  >  ANG_MAX) nave.angVert  =  ANG_MAX;
    else if (nave.angVert  < -ANG_MAX) nave.angVert  = -ANG_MAX;
    if      (nave.angHoriz >  ANG_MAX) nave.angHoriz =  ANG_MAX;
    else if (nave.angHoriz < -ANG_MAX) nave.angHoriz = -ANG_MAX;
}
