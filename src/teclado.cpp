#include <cctype>  /* toupper */

#include "teclado.hpp"
#include "nave.hpp"
#include "cenario.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO   ' '  /* barra de espaços */
#define TECLA_TIRO2  'Z'
#define TECLA_CAMERA 'C'
#define TECLA_PAUSA  'P'
#define TECLA_FPS    'F'
#define TECLA_SAIDA  'Q'

/* Vetores para reconhecimento de teclado */
static bool        keyStates[128] = {false};
static bool keySpecialStates[128] = {false};

/* Variáveis booleanas indicadoras */
static bool primeiraPessoa = false;
static bool       exibeFPS = false;
static bool        pausado = false;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void keyPressed(unsigned char key, int x, int y)
{
    keyStates[toupper(key)] = true;

    if (keyStates[TECLA_CAMERA]) primeiraPessoa = not primeiraPessoa;
    if (keyStates[TECLA_FPS])          exibeFPS = not exibeFPS;
    if (keyStates[TECLA_PAUSA])         pausado = not pausado;
    if (keyStates[TECLA_SAIDA]) encerraJogo();
}
 
void keyUp(unsigned char key, int x, int y)
{  
    keyStates[toupper(key)] = false;
}

void keySpecialPressed(int key, int x, int y)
{
    keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y)
{
    keySpecialStates[key] = false;
}

/*------------------------------------------------------------------*/

void keyOperations()
{
    if ((keyStates[TECLA_TIRO] or keyStates[TECLA_TIRO2])
            and Nave::getNave()->reduzEspera() <= 0) {
        Nave::getNave()->dispara();
    }
}

void keySpecialOperations()
{
    /* Taxa de alteração de ângulo por comando do usuário */
    static constexpr double ANG_MANUAL = ANG_MAX/20;

    Nave *nave = Nave::getNave();

    if      (keySpecialStates[GLUT_KEY_UP])    nave->angVert  += ANG_MANUAL;
    else if (keySpecialStates[GLUT_KEY_DOWN])  nave->angVert  -= ANG_MANUAL;
    if      (keySpecialStates[GLUT_KEY_LEFT])  nave->angHoriz -= ANG_MANUAL;
    else if (keySpecialStates[GLUT_KEY_RIGHT]) nave->angHoriz += ANG_MANUAL;

    /* Ângulos devem permanecer no intervalo [-ANG_MAX, ANG_MAX] */
    if      (nave->angVert  >  ANG_MAX) nave->angVert  =  ANG_MAX;
    else if (nave->angVert  < -ANG_MAX) nave->angVert  = -ANG_MAX;
    if      (nave->angHoriz >  ANG_MAX) nave->angHoriz =  ANG_MAX;
    else if (nave->angHoriz < -ANG_MAX) nave->angHoriz = -ANG_MAX;
}

/*------------------------------------------------------------------*/

bool estaEmPrimeiraPessoa() { return primeiraPessoa; }
bool estaPausado()          { return pausado;        }
bool exibindoFPS()          { return exibeFPS;       }
