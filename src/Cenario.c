#include <stdio.h>   /* puts, printf, system */
#include <stdlib.h>  /* exit */
#include "Cenario.h"
#include "Teclado.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Elementos básicos do jogo */
Nave nave;
Lista *inimigos;
Lista *projeteis;

static void imprimeElementos();

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

void atualiza()
{
    static int cont = TEMPO_INIMIGOS;
    static int t0 = 0;

    int dt = glutGet(GLUT_ELAPSED_TIME) - t0;

    if (dt < 1000/FPS) return;
    t0 += dt;

    /* Reconhecimento do teclado */
    keyOperations();
    if (!pausa) keySpecialOperations();

    if (!pausa) {
        /* Ações relacionadas à nave */
        moveNave();
        if (nave.invencibilidade > 0) (nave.invencibilidade)--;

        /* Loop para tratar de inimigos */
        Celula *p = inimigos;
        while (p->prox != NULL) {
            Inimigo *foe = p->prox->item;
            if (naveColidiu(foe)) danificaNave(DANO_COLISAO);
            if ((foe->base.espera)-- == 0) inimigoDispara(foe);
            if (inimigoSaiu(foe)) exclui(p);
            else p = p->prox;
        }

        /* Loop para verificar estado dos projéteis */
        p = projeteis;
        while (p->prox != NULL) {
            Projetil *bullet = p->prox->item;
            moveProjetil(bullet);
            if (verificaAcerto(bullet) || projetilSaiu(bullet)) exclui(p);
            else p = p->prox;
        }

        if (nave.vidas <= 0) encerraJogo();
        if (--cont == 0) {
            geraInimigo();
            cont = TEMPO_INIMIGOS;
        }
        /*imprimeElementos();*/
    }
    
    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

void encerraJogo()
{
    liberaLista(inimigos);
    liberaLista(projeteis);
    liberaTexturas();

    exit(EXIT_SUCCESS);
}

/*------------------------------------------------------------------*
 *
 *  Mostra informação a respeito dos elementos do jogo no
 *  timestep atual. Usada para depuração.
 *
 */
static void imprimeElementos()
{
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
    
    /* Para efeitos de clareza, todas as componentes z dos
       inimigos e projéteis são relativas à nave (e não absolutas). */
    puts("\n{Inimigos}");
    puts("    ( x, y, z)          Recarga    Precisão    Energia ");
    puts("-------------------     -------    --------   ---------");
    for (Celula *p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%4g, %3g, %4g)       %2d/%2d       %3.0f%%       %2d/%2d\n",
            foe->base.x, foe->base.y, (foe->base.z - nave.base.z),
            foe->base.espera, foe->base.cooldown, 100 * foe->precisao,
            foe->base.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("     ( x, y, z)            [ vx, vy, vz]         Amigo? ");
    puts("-------------------    --------------------     --------");
    for (Celula *p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%4.0f, %3.0f, %4.0f)      [%4.1f, %4.1f, %5.1f]        %s\n",
            bullet->x, bullet->y, (bullet->z - nave.base.z),
            bullet->vx, bullet->vy, bullet->vz,
            (bullet->amigo) ? "sim" : "não");
    }
}
