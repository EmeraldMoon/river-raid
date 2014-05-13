/*------------------------------------------------------------------*
 |                                                                  |
 |  Definições e estruturas comuns a serem usadas                   |
 |  em mais de um módulo do projeto.                                |
 |                                                                  |
 *------------------------------------------------------------------*/

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Pi, nossa constante querida */
#define PI 3.14159265359

/* Calcula quadrado de x */
#define sq(x) (pow((x), 2))

/* Atalhos para acessar os atributos */
#define base.x x
#define base.y y
#define base.z z
#define base.hp hp
#define base.cooldown cooldown
#define base.espera   espera

/*
 *  Atributos comuns entre nave e inimigos.
 */
typedef struct esqueleto Esqueleto;
struct esqueleto
{
    int x;  /* posição horizontal (centro == 0) */
    int y;  /* altura em relação ao solo */
    int z;  /* distância desde o início do cenário */
    
    /* Se (hp <= 0), elemento é destruído */
    int hp;
    
    /* Timesteps de espera entre um tiro e outro */
    unsigned int cooldown;  /* valor máximo, fixo */
    unsigned int espera;    /* sofre decremento até chegar a 0 */
};
