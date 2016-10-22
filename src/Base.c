#include <stdio.h>  /* perror */
#include <stdlib.h>
#include <string.h>

#include "Base.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void *mallocSafe(size_t nbytes)
{
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        perror("mallocSafe()");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/*------------------------------------------------------------------*/

void leVetores(GLdouble vetores[][3], GLsizei n, char nomeArq[])
{
    char caminho[strlen("model/") + strlen(nomeArq) + 1];
    sprintf(caminho, "model/%s", nomeArq);

    FILE *arq = fopen(caminho, "r");
    if (arq == NULL) {
        fprintf(stderr, "ERRO: arquivo \"%s\" não encontrado\n", nomeArq);
        exit(EXIT_FAILURE);
    }
    for (GLsizei i = 0; i < n; i++) {
        for (GLsizei j = 0; j < 3; j++) {
            fscanf(arq, "%lf", &vetores[i][j]);
        }
    }
    fclose(arq);
}
