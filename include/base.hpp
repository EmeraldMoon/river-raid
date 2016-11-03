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
class Corpo
{
public:
    /* Posição do corpo no cenário */
    double x;  /* posição horizontal (centro == 0) */
    double y;  /* altura em relação ao solo */
    double z;  /* distância desde o ponto inicial do jogo */

    /* Dimensões do corpo.
       No caso de esféricos, altura == 2 * raio. */
    double raio, altura;

    /*
     *  Gera um corpo de forma randômica com posições
     *  aleatórias x e y e coordenada fixa z.
     */
    Corpo(double z);

    /*
     *  Verifica se ocorreu colisão entre dois corpos.
     *  Os corpos são considerados cilíndricos, para facilitar.
     */
    bool colidiuCom(Corpo *corpo);

    /*
     *  Verifica se o corpo saiu por um dos limites da tela.
     */
    bool saiu();
};

/*
 *  Unidade de batalha do jogo, com hp e tiros recarregáveis.
 *  Nave e inimigos são subclasses desta.
 */
class Unidade : public Corpo
{
public:
    /* Se (hp <= 0), elemento é destruído */
    int hp;

    /* Timesteps de espera entre um tiro e outro.
       cooldown é fixo, espera é decrementada de cooldown até 0. */
    int cooldown, espera;

    Unidade(double z);
};
