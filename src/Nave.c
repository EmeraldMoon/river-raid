#include <math.h>  /* tan */
#include "Cenario.h"
#include "Nave.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaNave()
{
    /* Coordenadas iniciais */
    nave.x = 0;
    nave.y = Y_MAX/2;
    nave.z = 0;

    /* Começa apontando para o centro */
    nave.horizontal = 0.0;
    nave.vertical   = 0.0;

    nave.velocidade = VEL_INI;
    nave.hp = NAVE_HP;
}

void moveNave()
{
    nave.x += nave.velocidade * tan(nave.horizontal);
    nave.y += nave.velocidade * tan(nave.vertical);
    nave.z += nave.velocidade;
}
