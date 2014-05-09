/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo de testes para a primeira parte do projeto.              |
 |  Simula um jogo básico, ainda sem interação com o usuário,       |
 |  e exibe informações na tela toda vez que a nave for atingida.   |
 |                                                                  |
 |  Uso: ./bin/River [semente]                                      |
 |  - semente: inteiro para o gerador de nºs aleatórios. Caso       |
 |             nenhuma seja digitada, usa-se o relógio do sistema.  |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdio.h>   /* printf, puts, getchar */
#include <stdlib.h>  /* system, rand, srand */
#include <time.h>    /* time */
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Cenario.h"

/* Tempo de espera entre um inimigo e outro */
#define TEMPO_INIMIGOS 10

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

/*
 *  Recebe um timestep, limpa a tela e imprime informação a respeito de
 *  todos os elementos do jogo neste timestep. É aguardado que o usuário
 *  pressione [Enter] e então espera-se até a próxima chamada da função.
 *
 *  Para efeitos de clareza, todas as componentes Z, exceto a da nave,
 *  são relativas à nave em si (e não absolutas).
 */
void imprimeElementos(int timestep)
{
    Celula *p;

    #ifdef __linux__
        system("clear");
    #elif  _WIN32
        system("cls");
    #endif

    printf("Timestep %d\n", timestep);
    puts("==============");
    puts("{Nave}");
    printf("Energia: %-3d/%d\n", nave.hp, NAVE_HP);
    printf("Posição: (%d, %d, %d)\n", nave.x, nave.y, nave.z);
    
    puts("\n{Inimigos}");
    puts("   ( x, y, z)       Recarga    Precisão  ");
    puts("----------------    -------    --------  ");
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%3d, %2d, %3d)      %2d/%2d       %3.0f%%\n",
            foe->x, foe->y, (foe->z - nave.z),
            foe->espera, foe->cooldown, 100*foe->precisao);
    }
    puts("\n{Projéteis}");
    puts("   ( x, y, z)          [ vx, vy, vz]     ");
    puts("----------------    -------------------- ");
    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%3.0f, %2.0f, %3.0f)      [%4.1f, %4.1f, %4.1f] \n",
            bullet->x, bullet->y, (bullet->z - nave.z),
            bullet->vel.x, bullet->vel.y, bullet->vel.z);
    }
    getchar();
}

/*
 *  == MAIN ==
 *  Tudo começa por aqui.
 *  Tudo também termina por aqui (geralmente).
 */
int main(int argc, char **argv)
{
    int semente;
    int timestep, cont;
    int hpAtual;

    if (argc < 2) semente = time(NULL);
    else          semente = atoi(argv[1]);

    inicializa();
    srand(semente);
    hpAtual = nave.hp;    

    cont = TEMPO_INIMIGOS;
    for (timestep = 1; nave.hp > 0; timestep++) {
        atualizaCenario();

        if (cont == 0) {
            geraInimigo();
            cont = TEMPO_INIMIGOS;
        }
        else cont--;

        if (nave.hp < hpAtual) {
            imprimeElementos(timestep);
            hpAtual = nave.hp;
        }
    }
    liberaCenario();

    return EXIT_SUCCESS;
}
