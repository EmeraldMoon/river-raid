/*------------------------------------------------------------------*
 |                                                                  |
 |  Módulo de testes para a segunda parte do projeto.               |
 |  Simula um jogo básico, recebendo comandos do usuário e          |
 |  exibindo informações na tela a respeito dos elementos do jogo.  |
 |                                                                  |
 |  Uso: ./bin/River [semente] [intervalo para comandos]            |
 |                                                                  |
 *------------------------------------------------------------------*/

#include <stdio.h>   /* printf, puts, getchar */
#include <stdlib.h>  /* system, rand, srand */
#include <time.h>    /* time */
#include <ctype.h>   /* toupper */
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Cenario.h"

/* Tempo de espera entre um inimigo e outro */
#define TEMPO_INIMIGOS  10
#define TEMPO_COMANDO   40

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

/*------------------------------------------------------------------*
 *
 *  Recebe uma letra correspondente à uma tecla digitada pelo
 *  usuário e executa o comando correspondente.
 *
 */
void executaComando(char tecla)
{
    tecla = toupper(tecla);

    switch (tecla) {
        case 'U': nave.angY += ANG_MANUAL; break;
        case 'D': nave.angY -= ANG_MANUAL; break;
        case 'L': nave.angX -= ANG_MANUAL; break;
        case 'R': nave.angX += ANG_MANUAL; break;

        case 'Z': naveDispara(); break;
    }

    /* Ângulos nunca serão maiores que ANG_MAX */ 
    if (nave.angX > ANG_MAX) nave.angX = ANG_MAX;
    if (nave.angY > ANG_MAX) nave.angY = ANG_MAX;
}

/*------------------------------------------------------------------*
 *
 *  Recebe um timestep e mostra informação a respeito de todos os
 *  elementos do jogo neste momento. Após usuário pressionar [Enter] 
 *  aguarda-se até a próxima chamada da função.
 *
 *  Para efeitos de clareza, todas as componentes Z, exceto a da
 *  nave, são relativas à nave em si (e não absolutas).
 *
 */
void imprimeElementos(int timestep)
{
    Celula *p;

    /* Limpa a tela */
    #ifdef __linux__
        system("clear");
    #elif  _WIN32
        system("cls");
    #endif

    printf("Timestep %d\n", timestep);
    puts("==============");
    puts("{Nave}");
    printf("PONTUAÇÂO: %d\n", nave.score);
    printf("VIDAS: %d\n", nave.vidas);
    printf("Energia: %-3d/%d\n", nave.base.hp, NAVE_HPMAX);
    printf("Posição: (%g, %g, %g)\n", 
        nave.base.x, nave.base.y, nave.base.z);
    
    puts("\n{Inimigos}");
    puts("   ( x, y, z)       Recarga    Precisão    Energia ");
    puts("----------------    -------    --------   ---------");
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%3g, %2g, %3g)      %2d/%2d       %3.0f%%      %3d/%3d\n",
            foe->base.x, foe->base.y, (foe->base.z - nave.base.z),
            foe->base.espera, foe->base.cooldown, 100 * foe->precisao,
            foe->base.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("   ( x, y, z)          [ vx, vy, vz]       Amigo");
    puts("----------------    --------------------   -----");
    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%3.0f, %2.0f, %3.0f)      [%4.1f, %4.1f, %4.1f]      %d\n",
            bullet->x, bullet->y, (bullet->z - nave.base.z),
            bullet->vx, bullet->vy, bullet->vz, bullet->amigo);
    }
}

/*----------------*-------------------------------------------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    int semente, tempoComando;
    int timestep, cont;

    if (argc < 2) semente = time(NULL);
    else          semente = atoi(argv[1]);

    if (argc < 3) tempoComando = TEMPO_COMANDO;
    else          tempoComando = atoi(argv[2]);

    inicializa();
    srand(semente);
    cont = TEMPO_INIMIGOS; 

    /* Loop principal de execução */
    for (timestep = 1; nave.vidas > 0; timestep++) {
        atualizaCenario();

        if (timestep % tempoComando == 0) {
            imprimeElementos(timestep);
            executaComando(getchar());
        }

        if (cont == 0) {
            geraInimigo();
            cont = TEMPO_INIMIGOS;
        }
        else cont--;
    }
    
    liberaCenario();

    return EXIT_SUCCESS;
}
