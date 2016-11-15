#include <array>
#include <string>
#include <fstream>    /* ifstream */
#include <algorithm>  /* count */
#include <iostream>
#include <GL/freeglut.h>

#include "modelo.hpp"
#include "cenario.hpp"

/*-----------------*
 |   M O D E L O   |
 *-----------------*------------------------------------------------*/

Modelo::Modelo(std::string nomeArq)
{
    /* Caminho do arquivo de modelo */
    std::string caminho = "model" + ("/" + nomeArq);

    /* Abre o arquivo */
    std::ifstream arq;
    try { arq.open(caminho); }
    catch (...) {
        std::cerr << "Erro ao abrir arquivo " << caminho << "\n";
        return;
    }
    /* Obtém número de linhas do arquivo */
    GLsizei n = std::count(std::istreambuf_iterator<char>(arq),
                           std::istreambuf_iterator<char>(), '\n');
    arq.seekg(0);

    /* Lê todas as coordenadas */
    coords.resize(3 * n);
    for (auto &coord : coords) arq >> coord;
}

/*-------------------*
 |   T E X T U R A   |
 *-------------------*----------------------------------------------*/

Textura::Textura(std::string nomeArq, bool mipmap)
{
    /* Caminho do arquivo de textura */
    std::string caminho = "texture" + ("/" + nomeArq);

    /* Carrega textura do arquivo */
    if (!texture.loadFromFile(caminho)) {
        std::cerr << "ERRO: Não foi possível ler textura em "
                  << caminho << "\n";
    }
    if (mipmap) texture.generateMipmap();
    texture.setSmooth(true);
}

void Textura::ativa()
{
    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(&texture);
}
