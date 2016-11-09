/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do carregamento e manuseio de texturas da parte gráfica.  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <GL/freeglut.h>

/*------------------------------------------------------------------*/

/* Diretórios para modelos e texturas */
#define   MODEL_DIR   "model"
#define TEXTURE_DIR "texture"

/*
 *  Modelo de um objeto desenhável do jogo.
 */
struct Modelo
{
    /* Identificador de textura */
    GLuint texturaId;

    /* Vetor de coordenadas de vértices */
    std::vector<GLdouble> coords;
};

/*------------------------------------------------------------------*/

/*
 *  Recebe um nome de arquivo contendo vértices.
 *  Carrega os vértices do arquivo para serem usados no modelo.
 */
void leVertices(std::string nomeArq, Modelo &modelo);

/*
 *  Recebe um nome de arquivo em formato PPM.
 *  Carrega a textura deste arquivo para a memória.
 *  mipmap indica se será usado o recurso de mipmapping.
 */
void carregaTextura(std::string nomeArq, GLboolean mipmap, Modelo &modelo);

/*
 *  Libera textura atrelada ao OpenGL.
 */
void liberaTextura(Modelo &modelo);
