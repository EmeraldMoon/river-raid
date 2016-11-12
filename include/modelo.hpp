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

/*
 *  Modelo de um objeto desenhável do jogo.
 */
class Modelo
{
private:
    /* Diretório onde texturas estão localizadas */
    static constexpr char DIR[] = "model";

public:
    /* Vetor de coordenadas de vértices */
    std::vector<GLdouble> coords;

    /*
     *  Lê coordenadas de um arquivo de vértices.
     */
    Modelo(std::string nomeArq);
};

/*------------------------------------------------------------------*/

class Textura
{
private:
    /* Diretório onde texturas estão localizadas */
    static constexpr char DIR[] = "texture";

    /* Identificador de textura */
    GLuint id;

public:
    /*
     *  Recebe um nome de arquivo em formato PPM.
     *  Carrega a textura deste arquivo para a memória.
     *  mipmap indica se será usado o recurso de mipmapping.
     */
    Textura(std::string nomeArq, GLboolean mipmap);

    /*
     *  Ativa textura para ser desenhada em determinado objeto.
     */
    void ativa() const;
};
