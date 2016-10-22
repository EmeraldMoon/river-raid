/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação dos elementos de defesa, os inimigos do jogo.       |
 |  Cuida de geração, remoção, disparo de projéteis, etc.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>
#include "Lista.h"
#include "Base.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Tempo de espera até criar um inimigo */
#define TEMPO_INIMIGOS 150

/* Atributos padrão */
#define FOE_HPMAX   80
#define FOE_RAIO    40
#define FOE_ALTURA 100

/* Pontuação */
#define PONTOS_ACERTO    100
#define PONTOS_DESTRUCT 1000

/* Arquivo e número de vértices referentes ao modelo das defesas */
#define DEFESA_MODELO "Defesa.vert"
#define DEFESA_NUM_VERTICES 15538

/* Idem, para normais */
#define DEFESA_NORMAL_MODELO "Defesa.norm"
#define DEFESA_NUM_NORMAIS 7541

/*
 *  Representa um inimigo (elemento de defesa).
 */
typedef struct inimigo Inimigo;
struct inimigo
{
    Corpo base;

    /* Chance (esperada) de um projétil disparado acertar a nave */
    double precisao;
};

extern Lista *inimigos;

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*/

/*
 *  Carrega modelo gráfico de inimigos para a memória.
 */
void carregaModeloInimigos();

/*
 *  Insere um inimigo na respectiva lista.
 */
void criaInimigo(Inimigo foe);

/*
 *  Gera uma torre ao fundo do cenário, com altura, posição
 *  horizontal e outros atributos aleatórios.
 */
void geraInimigo();

/*
 *  Dispara um projétil em direção à nave e atualiza o tempo
 *  de recarga do inimigo. A fim de aumentar a chance de acerto,
 *  os tiros são mirados logo à frente da posição da nave.
 */
void inimigoDispara(Inimigo *foe);

/*
 *  Verifica se o inimigo passou pela nave e saiu
 *  da tela (possivelmente sem ser destruído).
 */
bool inimigoSaiu(Inimigo *foe);

/*
 *  Recebe um inimigo e o desenha na tela.
 */
void desenhaInimigo(Inimigo *foe);
