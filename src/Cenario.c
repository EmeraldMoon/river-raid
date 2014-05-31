#include <stdio.h>  /* puts, printf, system */
#include "Cenario.h"
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Random.h"

/* Elementos básicos do jogo */
Nave nave;
Lista *inimigos;
Lista *projeteis;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void inicializaCenario()
{
    criaNave(0, VIDAS_INI);
    inimigos  = criaLista();
    projeteis = criaLista();
    nave.score = 0;
}

/*------------------------------------------------------------------*/

void atualizaCenario()
{
    Celula *p;

    /* Ações relacionadas à nave */
    moveNave();
    desenhaNave();
    if (nave.invencibilidade > 0) (nave.invencibilidade)--;

    /* Loop para verificar estado dos projéteis */
    p = projeteis;
    while (p->prox != NULL) {
        Projetil *bullet = p->prox->item;
        moveProjetil(bullet);
        desenhaProjetil(bullet);
        if (verificaAcerto(bullet) || projetilSaiu(bullet)) exclui(p);
        else p = p->prox;
    }

    /* Loop para tratar de inimigos */
    p = inimigos;
    while (p->prox != NULL) {
        Inimigo *foe = p->prox->item;
        if (naveColidiu(foe)) danificaNave(DANO_COLISAO);
        if ((foe->base.espera)-- == 0) inimigoDispara(foe);
        desenhaInimigo(foe);
        if (inimigoSaiu(foe)) exclui(p);
        else p = p->prox;
    }
}

/*------------------------------------------------------------------*
 *  
 *  Esta função é um modelo de testes. No futuro, a geração
 *  de inimigos deve ser feita de forma pré-determinada.
 *
 */
void geraInimigo()
{
    Inimigo foe;

    /* (uniforme(-1, 0) | 1) gera um número que é 1 ou -1 */
    foe.base.x = uniforme(X_MAX/2, X_MAX) * (uniforme(-1, 0) | 1);
    foe.base.y = uniforme(0, Y_MAX/2);
    foe.base.z = nave.base.z + Z_MAX;

    foe.base.hp       = FOE_HPMAX;
    foe.base.cooldown = uniforme(10, 20);
    foe.base.espera   = foe.base.cooldown;
    foe.base.raio     = FOE_RAIO;
    foe.base.altura   = 2 * foe.base.y;
    foe.precisao      = uniformeD(0.5, 1.0);

    criaInimigo(foe);
}

/*------------------------------------------------------------------*/

void liberaCenario()
{
    liberaLista(inimigos);
    liberaLista(projeteis);
}

/*------------------------------------------------------------------*/

void imprimeElementos()
{
    Celula *p;

    /* Limpa a tela do terminal/prompt */
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #endif    

    puts("{Nave}");
    printf("PONTUAÇÂO: %d\n", nave.score);
    printf("VIDAS: %d\n", nave.vidas);
    printf("Energia: %-3d/%d\n", nave.base.hp, NAVE_HPMAX);
    printf("Posição: (%.0f, %.0f, %.0f)\n", 
        nave.base.x, nave.base.y, nave.base.z);
    printf("Ângulos: (%.0f°, %.0f°)\n",
        (180/PI) * nave.angHoriz, (180/PI) * nave.angVert);
    
    /* Para efeitos de clareza, todas componentes Z dos inimigos
       e projéteis são relativas à nave (e não absolutas). */
    puts("\n{Inimigos}");
    puts("   ( x, y, z)       Recarga    Precisão    Energia ");
    puts("----------------    -------    --------   ---------");
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%3g, %3g, %3g)      %2d/%2d       %3.0f%%       %2d/%2d\n",
            foe->base.x, foe->base.y, (foe->base.z - nave.base.z),
            foe->base.espera, foe->base.cooldown, 100 * foe->precisao,
            foe->base.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("   ( x, y, z)          [ vx, vy, vz]        Amigo? ");
    puts("----------------    --------------------   --------");
    for (p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%3.0f, %3.0f, %3.0f)      [%4.1f, %4.1f, %4.1f]       %s\n",
            bullet->x, bullet->y, (bullet->z - nave.base.z),
            bullet->vx, bullet->vy, bullet->vz,
            (bullet->amigo) ? "sim" : "não");
    }
}
