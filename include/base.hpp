/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições de uso geral e estruturas comuns, a serem            |
 |  usadas em mais de um módulo do projeto.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Pi, nossa constante querida */
#define PI 3.14159265359

/* Calcula norma de um vetor */
#define norma(x, y, z) sqrt((x)*(x) + (y)*(y) + (z)*(z))

/*------------------------------------------------------------------*/

/*
 *  Superclasse básica de um elemento físico do jogo.
 *  A ser usada comumente entre todos os objetos.
 */
struct Corpo
{
    /* Posição do no cenário */
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o início do cenário */

    /* Dimensões do.
       No caso de esféricos, altura == 2 * raio. */
    double raio, altura;
};

/*
 *  Unidade de batalha do jogo, com hp e tiros recarregáveis.
 *  Nave e inimigos são subclasses desta.
 */
struct Unidade : public Corpo
{
    /* Se (hp <= 0), elemento é destruído */
    int hp;

    /* Timesteps de espera entre um tiro e outro.
       cooldown é fixo, espera é decrementada de cooldown até 0. */
    int cooldown, espera;
};

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Posiciona um de forma randômica com posições
 *  aleatórias x e y e coordenada fixa z.
 */
void geraCorpo(Corpo *corpo, double z);

/*
 *  Verifica se ocorreu colisão entre doiss.
 *  Os doiss são considerados cilíndricos, para facilitar.
 */
bool ocorreuColisao(Corpo *a, Corpo *b);

/*
 *  Verifica se o saiu por um dos limites da tela.
 *  naveZ refere-se à posição z da nave neste instante.
 */
bool corpoSaiu(Corpo *corpo, double naveZ);
