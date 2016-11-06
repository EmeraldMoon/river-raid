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

/* Tempo de espera até criar um inimigo */
#define TEMPO_INIMIGOS 120

/* Atributos padrão */
#define FOE_HPMAX     80
#define FOE_RAIO      40
#define DANO_COLISAO  20

/* Pontuação */
#define PONTOS_ACERTO      100
#define PONTOS_DESTRUICAO 1000

/* Tempo que um inimigo pisca após ser acertado */
#define FOE_TEMPO_DANO 30

/*------------------------------------------------------------------*/

/*
 *  Representa um inimigo (elemento de defesa).
 */
class Inimigo : public Unidade
{
private:
    void aplicaPrecisao(double *vx, double *vy, double *vz);

    /* Chance (esperada) de um projétil disparado acertar a nave */
    double precisao;

    /* Dano causado à nave ao colidir com inimigo */
    int danoColisao;

    /* Pontuação conferida ao jogador em caso de acerto e destruição */
    int pontosAcerto;
    int pontosDestruicao;

    /* Para fins visuais */
    int tempoDano;

public:
    static Lista<Inimigo> lista;

    /*
     *  Cria um novo inimigo na pósição z do cenário.
     */
    Inimigo(double z);

    /*
     *  Dispara um projétil em direção à nave e
     *  reinicia o tempo de recarga do inimigo.
     */
    void dispara(Nave *nave);

    /*
     *  Danifica o inimigo.
     */
    void danifica(int dano);

    /*
     *  Desenha o inimigo na tela.
     */
    void desenha();

    /* Getters */
    double getPrecisao();
    int    getDanoColisao();
    int    getPontosAcerto();
    int    getPontosDestruicao();
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
