/*------------------------------------------------------------------*
 |                                                                  |
 |  Interface para trabalhar com projéteis, seja dos inimigos ou    |
 |  do próprio jogador. Lida desde a verificação de posição (saiu   |
 |  da tela, acertou um alvo, etc), até o cálculo da trajetória,    |
 |  baseado na precisão do inimigo e localização do alvo.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "base.hpp"

/*------------------------------------------------------------------*/

/*
 *  Representa um projétil causador de dano.
 */
class Projetil : public Corpo
{
private:
    /* Componentes de velocidade do tiro */
    double vx, vy, vz;

    /* Energia removida do elemento em caso de acerto */
    int dano = 15;

    /* Indica se tiro foi disparado pela nave */
    bool amigo;

public:
    /* Módulo de velocidade padrão de um projétil */
    static constexpr double VEL_PADRAO = 9.0;

    /*
     *  Constrói um projétil tomando como base uma unidade, um vetor
     *  velocidade (vx, vy, vz) e um booleano amigo indicando se foi
     *  disparado por nave ou inimigo. Posiciona-o externamente à unidade.
     */
    Projetil(Unidade &uni, double vx, double vy, double vz,
             bool amigo);

    /*
     *  Atualiza a posição do projétil em relação ao frame anterior.
     */
    void move();

    /*
     *  Recebe um projétil. Se colisão for verificada com algum elemento,
     *  remove energia deste e, caso seja um inimigo, verifica também
     *  se o mesmo foi destruído. Caso tiro tenho sido dado pela nave,
     *  são dados pontos ao jogador por acerto e por destruição.
     *
     *  A função devolve um booleano indicando se houve acerto.
     */
    bool verificaAcerto();

    /*
     *  Recebe um projétil e o desenha na tela.
     */
    void desenha();

    /* Getters */
    double getVx();
    double getVy();
    double getVz();
    int    getDano();
    bool   isAmigo();
};
