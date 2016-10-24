#include <stdio.h>   /* fopen, fclose */
#include <stdlib.h>  /* exit */
#include <string.h>  /* strlen, sprintf */
#include <GL/freeglut.h>

#include "Modelo.h"

/*-------------------*----------------------------------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void leVetores(GLdouble vetores[][3], GLsizei n, char nomeArq[])
{
    char caminho[strlen(MODELO_DIR) + strlen(nomeArq) + 1];
    sprintf(caminho, "%s%s", MODELO_DIR, nomeArq);

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
