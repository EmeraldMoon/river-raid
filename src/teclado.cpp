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
            and Nave::get()->reduzEspera() <= 0) {
        Nave::get()->dispara();
    }
}

void keySpecialOperations()
{
    /* Faz operação entre booleanos para encontrar sentidos */
    int sentidoVert = keySpecialStates[GLUT_KEY_UP] -
                      keySpecialStates[GLUT_KEY_DOWN];
    int sentidoHoriz = keySpecialStates[GLUT_KEY_RIGHT] -
                       keySpecialStates[GLUT_KEY_LEFT];

    Nave::get()->atualizaVertical(sentidoVert);
    Nave::get()->atualizaHorizontal(sentidoHoriz);
}

/*------------------------------------------------------------------*/

bool estaEmPrimeiraPessoa() { return primeiraPessoa; }
bool estaPausado()          { return pausado;        }
bool exibindoFPS()          { return exibeFPS;       }
