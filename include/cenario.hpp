/*------------------------------------------------------------------*
 |                                                                  |
 |  Cuida do cenário visível e de seus elementos, lidando com       |
 |  inicialização, liberação e atualização do elementos do jogo.    |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <GL/freeglut.h>

#include "base.hpp"
#include "nave.hpp"
#include "defesa.hpp"
#include "tiro.hpp"
#include "item.hpp"
#include "modelo.hpp"

/*------------------------------------------------------------------*/

/*
 *  Faz a chamada das funções de atualização e desenho, além de
 *  controlar o tempo do jogo, evitando que o mesmo rode rápido
 *  demais e pulando frames que o computador não consiga exibir.
 *
 *  É usada a técnica de interpolação: o tempo de jogo é constante,
 *  mesmo que a taxa de desenho na tela passe por variações.
 *
 *  O parâmetro da função não é utilizado.
 */
void controlaTempo(int unused);

/*
 *  Devolve tempo entre penúltima e última atualizações.
 */
int getDelayTempo();

/*------------------------------------------------------------------*/

class Cenario
{
private:
    /* Ponto de acesso à classe (singleton) */
    static Cenario *cenario;

    /* Modelos do cenário */
    Modelo modeloRio;
    Modelo modeloParede;
    Modelo modeloFundo;

    /* Indica se serão impressas informações de debug */
    bool debug;

    void imprime();

    void desenhaFundo();
    void desenhaRio();
    void desenhaParede();
    void desenhaSuperficie(GLuint texture, GLdouble coords[4][2],
                           GLdouble vertex[4][3]);

public:
    /* Coordenadas máximas da área de jogo. */
    static constexpr double X_MAX = 360.0;
    static constexpr double Y_MAX = 200.0;

    /* Distância máxima visível */
    static constexpr double Z_DIST = 3000.0;

    /* Variação negativa de vy dos projéteis por timestep */
    static constexpr double ACEL_GRAVIDADE = 0.01;

    /* Elementos dinâmicos do jogo */
    Nave nave;
    Lista<Inimigo>  inimigos;
    Lista<Projetil> projeteis;
    Lista<Item>     itens;

    /* 
     *  Devolve única instância de classe.
     */ 
    static Cenario &get();

    /*
     *  Carrega modelos e texturas para a memória.
     */
    Cenario(bool godMode, bool debug);

    /*
     *  Cuida direta ou indiretamente de tudo que compõe o cenário,
     *  sendo responsável por atualizar posições, tratar interações
     *  entre objetos e encerrar o jogo quando cabível.
     */
    void atualiza();

    /*
     *  Desenha todos os elementos do cenário, desde os meramente
     *  visuais (rio, paredes e fundo) até os objetos do jogo
     *  (nave, inimigos, tiros e itens).
     */
    void desenha();

    /*
     *  Libera toda a memória alocada para as listas,
     *  texturas e vértices e encerra execução do jogo.
     */
    void encerraJogo();
};
