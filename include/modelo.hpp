/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do carregamento e manuseio de texturas da parte gráfica.  |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

/*------------------------------------------------------------------*/

/*
 *  Modelo de um objeto desenhável do jogo.
 */
class Modelo
{
public:
    /* Vetor de coordenadas de vértices */
    std::vector<double> coords;

    /*
     *  Lê coordenadas de um arquivo de vértices.
     */
    Modelo(std::string nomeArq);
};

/*------------------------------------------------------------------*/

/*
 *  Textura a ser aplicada em objetos do jogo.
 */
class Textura
{
private:
    /* Objeto de textura do SFML */
    sf::Texture texture;

public:
    /*
     *  Recebe um nome de arquivo de imagem.
     *  Carrega a textura deste arquivo para a memória.
     *  mipmap indica se será usado o recurso de mipmapping.
     */
    Textura(std::string nomeArq, bool mipmap);

    /*
     *  Ativa textura para ser desenhada em determinado objeto.
     */
    void ativa();
};
