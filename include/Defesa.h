/*------------------------------------------------------------------*
 |                                                                  |
 |  Manipulação dos elementos de defesa, os inimigos do jogo.       |
 |  Cuida de geração, remoção, disparo de projéteis, etc.           |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <stdbool.h>

#include "Base.h"
#include "Lista.h"
#include "Nave.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Tempo de espera até criar um inimigo */
#define TEMPO_INIMIGOS 150

/* Atributos padrão */
#define FOE_HPMAX     80
#define FOE_RAIO      40
#define FOE_ALTURA   100
#define DANO_COLISAO  20

/* Pontuação */
#define PONTOS_ACERTO      100
#define PONTOS_DESTRUICAO 1000

/* Arquivo e número de vértices referentes ao modelo das defesas */
#define DEFESA_MODELO_VERTICES "Defesa.vert"
#define DEFESA_NUM_VERTICES 15538

/* Idem, para normais */
#define DEFESA_MODELO_NORMAIS "Defesa.norm"
#define DEFESA_NUM_NORMAIS 7541

/*------------------------------------------------------------------*/

/*
 *  Representa um inimigo (elemento de defesa).
 */
typedef struct inimigo Inimigo;
struct inimigo
{
    /* Corpo cilíndrico do inimigo */
    Corpo corpo;

    /* HP e cooldown */
    Atributos atribs;

    /* Chance (esperada) de um projétil disparado acertar a nave */
    double precisao;

    /* Dano causado à nave ao colidir com inimigo */
    int danoColisao;

    /* Pontuação conferida ao jogador em caso de acerto e destruição */
    int pontosAcerto;
    int pontosDestruicao;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Carrega informações de inimigos para a memória.
 *  Cria lista de inimigos.
 */
void carregaInimigos();

/*
 *  Gera uma torre na distância z, com altura, posição
 *  horizontal e outros atributos aleatórios.
 */
void geraInimigo(double z);

/*
 *  Dispara um projétil em direção à nave e
 *  reinicia o tempo de recarga do inimigo.
 */
void inimigoDispara(Inimigo *foe, Nave *nave);

/*
 *  Recebe um inimigo e o desenha na tela.
 */
void desenhaInimigo(Inimigo *foe);

/*
 *  Devolve ponteiro para a lista de inimigos.
 */
Lista *getListaInimigos();
