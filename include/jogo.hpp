/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções relacionadas à parte gráfica (OpenGL).                  |
 |  Cuida também do desenho de elementos fixos do cenário.          |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/glu.h>
#include <SFML/Graphics.hpp>

/*------------------------------------------------------------------*/

/* Tipo de luz a ser usada */
#define LUZ_AMBIENTE GL_LIGHT0

/* Distância da câmera à nave */
#define DIST_CAMERA 250

/* Constantes para os poliedros */
#define SLICES 16
#define STACKS 10

/*------------------------------------------------------------------*/

class Jogo
{
private:
    /* Janela gerenciada pelo SFML. RenderWindow é uma subclasse de
       Window que permite desenhar projeções fixas na tela. */
    sf::RenderWindow janela;

    /* Guarda fonte usada para texto */
    sf::Font fonte;

    /* Booleanos auto-explicativos */
    bool primeiraPessoa = false;
    bool exibindoFPS    = false;
    bool pausado        = false;

    void loop();

    void keyPressOperations(sf::Keyboard::Key tecla);
    void keyOperations();

    void desenha();
    void remodela();
    void exibeHud();
    void exibeFps();

public:
    /*
     *  Trata argumentos da linha de comando, inicializa elementos
     *  do jogo e passa para o loop de eventos.
     */
    Jogo(int argc, char *argv[]);
};
