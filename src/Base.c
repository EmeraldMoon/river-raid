#include <stdio.h>    /* perror */
#include <stdlib.h>   /* malloc, exit */
#include <stdbool.h>  /* bool */
#include <math.h>     /* hypot */

#include "Base.h"
#include "Random.h"
#include "Cenario.h"
#include "Grafico.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void posicionaCorpo(Corpo *corpo, double z)
{
    corpo->x = (X_MAX - corpo->raio) * uniforme(-1.0, 1.0);
    corpo->y = uniforme(Y_MAX/8, Y_MAX/2);
    corpo->z = z;
}

/*
 *  Em tese deveríamos fazer uma verificação diferente, na vertical,
 *  para caso um dos corpos fosse esférico, mas isso envolveria
 *  cálculos chatos (como verificar se esfera está contida
 *  em extremos do cilindro). Então, simplificamos.
 */
bool ocorreuColisao(Corpo *a, Corpo *b)
{
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    int dz = a->z - b->z;
    int somaRaios = a->raio + b->raio;

    /* Evita cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    /* Circunferências horizontais devem ser secantes. */
    return (abs(dy) < (a->altura + b->altura)/2
                && hypot(dx, dz) < somaRaios);
}

bool corpoSaiu(Corpo *corpo, double naveZ)
{
    /* O corpo saiu por um dos limites da tela (x, y ou z)?
       Caso a respostas seja sim, então o corpo saiu do jogo. */
    return (abs(corpo->x) > X_MAX)
        || (corpo->y < 0)
        || (corpo->z < naveZ - DIST_CAMERA || corpo->z > naveZ + Z_DIST);
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
