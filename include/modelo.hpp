/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do carregamento e manuseio de texturas da parte gráfica.  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Diretórios para modelos e texturas */
#define   MODEL_DIR   "model"
#define TEXTURE_DIR "texture"

/*
 *  Modelo de um objeto desenhável do jogo.
 */
typedef struct modelo Modelo;
struct modelo
{
    /* Vetor de coordenadas de vértices e número deles */
    GLdouble *coords;
    GLsizei numVertices;

    /* Identificador de textura */
    GLuint texturaId;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Recebe um nome de arquivo contendo vértices.
 *  Carrega os vértices do arquivo para serem usados no modelo.
 */
void leVertices(const char nomeArq[], Modelo *modelo);

/*
 *  Recebe um nome de arquivo em formato PPM.
 *  Carrega a textura deste arquivo para a memória.
 *  mipmap indica se será usado o recurso de mipmapping.
 */
void carregaTextura(const char nomeArq[], GLboolean mipmap, Modelo *modelo);

/*
 *  Libera memória alocada para os vértices do modelo.
 */
void liberaVertices(Modelo *modelo);

/*
 *  Libera textura atrelada ao OpenGL.
 */
void liberaTextura(Modelo *modelo);
