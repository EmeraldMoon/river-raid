#include <array>
#include <string>
#include <fstream>    /* ifstream */
#include <algorithm>  /* count */
#include <iostream>
#include <GL/freeglut.h>

#include "modelo.hpp"
#include "cenario.hpp"

/*-------------------*
 |   T E X T U R A   |
 *-------------------*----------------------------------------------*/

constexpr char Textura::DIR[];

/*------------------------------------------------------------------*/

static void ignoraComentario(std::ifstream &arq);
static void erro(std::ifstream &arq, std::string &nomeArq);

Textura::Textura(std::string nomeArq, GLboolean mipmap)
{
    /* Caminho do arquivo de textura */
    std::string caminho = std::string(DIR) + "/" + nomeArq;

    /* Abre o arquivo */
    std::ifstream arq;
    try { arq.open(caminho); }
    catch (...) {
        std::cerr << "Erro ao abrir arquivo " << caminho << "\n";
        Cenario::get().encerraJogo();
    }
    /* Faz verificação da chave mágica */
    std::string aux;
    std::getline(arq, aux);
    if (aux != "P6") erro(arq, caminho);

    /* Obtém largura e altura (ignore maxval) */
    GLsizei altura, largura;
    ignoraComentario(arq);
    arq >> altura >> largura;
    arq.ignore(2);

    /* Lê dados para uma string */
    std::vector<GLubyte> dados(3 * largura * altura);
    for (auto &dado : dados) dado = arq.get();

    /* Gera e guarda identificador de textura */
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    /* Carrega os dados (pixels) da textura */
    if (mipmap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, largura, altura,
                          GL_RGB, GL_UNSIGNED_BYTE, dados.data());
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura,
                     0, GL_RGB, GL_UNSIGNED_BYTE, dados.data());
    }
}

/*
 *  Pula comentários (iniciados com '#') na leitura do arquivo PPM.
 */
static void ignoraComentario(std::ifstream &arq)
{
    char c;
    for (;;) {
        arq >> std::skipws;
        arq.get(c);
        if (c != '#') break;
        std::string nada;
        std::getline(arq, nada);
    }
    arq.unget();
}

/*
 *  Fecha o programa quando a textura não é um arquivo PPM.
 */
static void erro(std::ifstream &arq, std::string &caminho)
{
    fprintf(stderr, "carregaTextura(): "
            "%s arquivo com formato inválido.\n", caminho.c_str());
    Cenario::get().encerraJogo();
}

/*------------------------------------------------------------------*/

void Textura::ativa() const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
}

/*-----------------*
 |   M O D E L O   |
 *-----------------*------------------------------------------------*/

constexpr char Modelo::DIR[];

/*------------------------------------------------------------------*/

Modelo::Modelo(std::string nomeArq)
{
    /* Caminho do arquivo de modelo */
    std::string caminho = std::string(DIR) + "/" + nomeArq;

    /* Abre o arquivo */
    std::ifstream arq;
    try { arq.open(caminho); }
    catch (...) {
        std::cerr << "Erro ao abrir arquivo " << caminho << "\n";
        Cenario::get().encerraJogo();
    }
    /* Obtém número de linhas do arquivo */
    GLsizei n = std::count(std::istreambuf_iterator<char>(arq),
                           std::istreambuf_iterator<char>(), '\n');
    arq.seekg(0);

    /* Lê todas as coordenadas */
    coords.resize(3 * n);
    for (auto &coord : coords) arq >> coord;
}
