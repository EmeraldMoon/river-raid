#include <stdio.h>    /* perror */
#include <stdlib.h>   /* malloc, exit */
#include <stdbool.h>  /* bool */
#include <math.h>     /* hypot */

#include "Base.h"
#include "Cenario.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

bool ocorreuColisao(Corpo *a, Corpo *b)
{
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    int dz = a->z - b->z;
    int somaRaios = a->raio + b->raio;

    /* Evita cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    /* TODO. */
    return (hypot(dx, dz) < somaRaios)
        && (abs(dy) < (a->altura + b->raio)/2);
}

bool corpoSaiu(Corpo *corpo, double naveZ)
{
    /* O corpo saiu por um dos limites da tela (x, y ou z)?
       Caso a respostas seja sim, então o corpo saiu do jogo. */
    return (abs(corpo->x) > X_MAX)
        || (corpo->y < 0)
        || (corpo->z < naveZ - DIST_CAMERA || corpo->z > naveZ + Z_MAX);
}

/*------------------------------------------------------------------*/

void *mallocSafe(size_t nbytes)
{
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        perror("mallocSafe()");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
