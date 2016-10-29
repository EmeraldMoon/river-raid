#include <stdio.h>   /* perror */
#include <string.h>  /* strcmp */
#include <GL/freeglut.h>

#include "Modelo.h"
#include "Base.h"
#include "Cenario.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void leVertices(const char nomeArq[], Modelo *modelo)
{
    /* Caminho do arquivo de modelo */
    char caminho[strlen(MODEL_DIR) + strlen(nomeArq) + 1];
    sprintf(caminho, "%s/%s", MODEL_DIR, nomeArq);

    /* Abre o arquivo */
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        perror("leVetores()");
        encerraJogo();
    }
    /* Obtém número de linhas do arquivo */
    GLsizei n = 0;
    while (fscanf(arq, " %*[^\n]") != EOF) n++;
    rewind(arq);

    /* Lê todas as coordenadas */
    GLdouble *coord = mallocSafe(3 * n * (sizeof coord[0]));
    for (GLsizei i = 0; i < 3 * n; i++) {
        fscanf(arq, "%lf", &coord[i]);
    }
    fclose(arq);

    /* Atualiza valores do modelo */
    modelo->coord = coord;
    modelo->numVertices = n;
}

/*------------------------------------------------------------------*/

static void ignoraComentario(FILE *arq);
static void erro(FILE *arq, const char *nomeArq);

void carregaTextura(const char nomeArq[], GLboolean mipmap, Modelo *modelo)
{
    /* Caminho do arquivo de textura */
    char caminho[strlen(TEXTURE_DIR) + strlen(nomeArq) + 1];
    sprintf(caminho, "%s/%s", TEXTURE_DIR, nomeArq);

    /* Abre o arquivo */
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        perror("carregaTextura()");
        encerraJogo();
    }
    /* Faz verificação da chave mágica */
    char aux[4];
    fscanf(arq, "%2s", aux);
    if (strcmp(aux, "P6") != 0) erro(arq, caminho);

    /* Obtém largura e altura (ignore maxval) */
    GLsizei altura, largura;
    ignoraComentario(arq);
    if (fscanf(arq, "%d %d %*d ", &altura, &largura) != 2) erro(arq, caminho);

    /* Obtém nº de bytes no restante do arquivo */
    long pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    GLsizei n = ftell(arq) - pos;
    fseek(arq, pos, SEEK_SET);

    /* Lê dados para uma string */
    GLubyte *dados = mallocSafe(n * (sizeof *dados));
    fread(dados, sizeof *dados, n, arq);
    fclose(arq);

    /* DOOM: Hell begins here */
    glGenTextures(1, &modelo->texturaId);
    glBindTexture(GL_TEXTURE_2D, modelo->texturaId);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mipmap) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, largura, altura,
                        GL_RGB, GL_UNSIGNED_BYTE, dados);
    } else {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura,
                    0, GL_RGB, GL_UNSIGNED_BYTE, dados);
    }
    free(dados);
}

/*
 *  Pula comentários (iniciados com '#') na leitura do arquivo PPM.
 */
static void ignoraComentario(FILE *arq)
{
    char c;
    for (;;) {
        fscanf(arq, " %c", &c);
        if (c != '#') break;
        fscanf(arq, "%*[^\n]");
    }
    ungetc(c, arq);
}

/*
 *  Fecha o programa quando a textura não é um arquivo PPM.
 */
static void erro(FILE *arq, const char *caminho)
{
    fprintf(stderr, "carregaTextura(): "
            "%s arquivo com formato inválido.\n", caminho);
    fclose(arq);
    encerraJogo();
}
