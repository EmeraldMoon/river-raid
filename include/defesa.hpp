/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação dos elementos de defesa, os inimigos do jogo.       |
 |  Cuida de geração, remoção, disparo de projéteis, etc.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "base.hpp"
#include "nave.hpp"

/*------------------------------------------------------------------*/

/*
 *  Representa um inimigo (elemento de defesa).
 */
class Inimigo : public Unidade
{
private:
    /* Dano causado à nave ao colidir com inimigo */
    static constexpr int DANO_COLISAO = 20;

    /* Pontuação conferida ao jogador em caso de acerto e destruição */
    static constexpr int PONTOS_ACERTO     = 100;
    static constexpr int PONTOS_DESTRUICAO = 1000;

    /* Chance (esperada) de um projétil disparado acertar a nave */
    double precisao;

    /* Para fins visuais */
    int tempoDano = 0;

    void aplicaPrecisao(double &vx, double &vy, double &vz);

public:
    /*
     *  Cria um novo inimigo na pósição z do cenário.
     */
    Inimigo(double z);

    /*
     *  Dispara um projétil em direção à nave e
     *  reinicia o tempo de recarga do inimigo.
     */
    void dispara(Nave &nave);

    /*
     *  Danifica o inimigo.
     */
    void danifica(int dano);

    /*
     *  Desenha o inimigo na tela.
     */
    void desenha();

    /* Getters */
    int    getDanoColisao();
    int    getPontosAcerto();
    int    getPontosDestruicao();
    double getPrecisao();
};

/*------------------------------------------------------------------*/

/*
 *  Carrega informações de inimigos para a memória.
 *  Cria lista de inimigos.
 */
void carregaInimigos();

/*
 *  Libera memória alocada para os inimigos.
 */
void liberaInimigos();
