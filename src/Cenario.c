#include <stdio.h>    /* puts, printf, system */
#include <stdlib.h>   /* exit */
#include <stdbool.h>  /* bool */
#include <GL/freeglut.h>

#include "Cenario.h"
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Item.h"
#include "Teclado.h"
#include "Textura.h"
#include "Grafico.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Indica se serão impressas informações de debug */
static bool debug = false;

/* Guarda intervalo entre chamadas de controlaTempo() */
static int dt;

static void imprimeElementos();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void inicializaJogo(bool godMode, bool _debug)
{
    /* Carrega listas, modelos e texturas */
    carregaNave(godMode);
    carregaInimigos();
    carregaProjeteis();
    carregaItens();
    
    debug = _debug;
}

/*------------------------------------------------------------------*/

void controlaTempo()
{
    static const int INTERVALO = 1000/FPS;
    static int t0 = 0, tExtra = 0;

    /* Obtém tempo desde última atualização */
    dt = glutGet(GLUT_ELAPSED_TIME) - t0;

    /* Limita FPS para programa não ir rápido demais (isso é mais por
       segurança. A opção de double buffered geralmente controla sozinha
       essa parte, mas é dependente do PC; logo, não podemos confiar). */
    if (dt < INTERVALO) {
        glutTimerFunc(INTERVALO - dt, controlaTempo, 0);
        return;
    }
    /* Acumula tempo extra gasto para desenhar */
    tExtra += dt - INTERVALO;

    /* Caso tempo acumulado chegue a um ou mais frames inteiros, 
       faz a interpolação deles entre o anterior e o próximo desenho. */
    for (;;) {
        if (!estaPausado()) atualizaCenario();
        if (tExtra < INTERVALO) break;
        tExtra -= INTERVALO;
    }
    /* Guarda o instante desta iteração */
    t0 = glutGet(GLUT_ELAPSED_TIME);

    /* Chama a função de desenho */
    glutPostRedisplay();
}

int getDelayTempo()
{
    return dt;
}

/*------------------------------------------------------------------*/

void atualizaCenario()
{
    static int contFoe  = TEMPO_INIMIGOS;
    static int contItem = TEMPO_ITEM;

    /* Reconhecimento do teclado */
    keyOperations();
    keySpecialOperations();

    /* Ações relacionadas à nave */
    Nave *nave = getNave();
    moveNave();
    if (nave->invencibilidade > 0) nave->invencibilidade--;

    /* Loop para tratar de inimigos */
    Celula *p = getListaInimigos();
    while (p->prox != NULL) {
        Inimigo *foe = p->prox->item;
        if (ocorreuColisao(&nave->corpo, &foe->corpo)) {
            danificaNave(foe->danoColisao);
        }
        if (--foe->atribs.espera == 0) inimigoDispara(foe, nave);
        if (corpoSaiu(&foe->corpo, nave->corpo.z)) listaRemove(p);
        else p = p->prox;
    }
    /* Loop para tratar de itens */
    Celula *q = getListaItens();
    while (q->prox != NULL) {
        Item *item = q->prox->item;
        if (ocorreuColisao(&nave->corpo, &item->corpo)) {
            ativaItem(item, nave);
            listaRemove(q);
        }
        else if (corpoSaiu(&item->corpo, nave->corpo.z)) listaRemove(q);
        else q = q->prox;
    }
    /* Loop para verificar estado dos projéteis */
    p = getListaProjeteis();
    while (p->prox != NULL) {
        Projetil *bullet = p->prox->item;
        moveProjetil(bullet);
        if (verificaAcerto(bullet) ||
                corpoSaiu(&bullet->corpo, nave->corpo.z)) {
            listaRemove(p);
        } else p = p->prox;
    }
    if (nave->vidas <= 0) encerraJogo();

    if (--contFoe <= 0) {
        geraInimigo(nave->corpo.z + Z_DIST);
        contFoe = TEMPO_INIMIGOS;
    }
    if (--contItem <= 0) {
        geraItem(nave->corpo.z + Z_DIST);
        contItem = TEMPO_ITEM;
    }
    if (debug) imprimeElementos();
}

/*------------------------------------------------------------------*/

void encerraJogo()
{
    liberaLista(getListaInimigos());
    liberaLista(getListaProjeteis());
    liberaLista(getListaItens());
    liberaTexturas();

    printf("Score final: %d\n", getNave()->score);
    exit(EXIT_SUCCESS);
}

/*------------------------------------------------------------------*/

/*
 *  Mostra informação a respeito dos elementos do jogo no
 *  timestep atual. Usada para depuração.
 */
static void imprimeElementos()
{
    /* Limpa a tela do terminal/prompt */
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #endif    

    Nave *nave = getNave();
    puts("{Nave}");
    printf("PONTUAÇÂO: %d\n", nave->score);
    printf("VIDAS: %d\n", nave->vidas);
    printf("Energia: %-3d/%d\n", nave->atribs.hp, NAVE_HPMAX);
    printf("Posição: (%.0f, %.0f, %.0f)\n", 
           nave->corpo.x, nave->corpo.y, nave->corpo.z);
    printf("Ângulos: (%.0f°, %.0f°)\n",
           180/PI * nave->angHoriz, 180/PI * nave->angVert);
    
    /* Para efeitos de clareza, todas as componentes z dos
       inimigos e projéteis são relativas à nave (e não absolutas). */
    puts("\n{Inimigos}");
    puts("    ( x, y, z)          Recarga    Precisão    Energia ");
    puts("-------------------     -------    --------   ---------");
    for (Celula *p = getListaInimigos(); p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%4.0f, %3.0f, %4.0f)       "
               "%2d/%3d       %3.0f%%       %2d/%2d\n",
               foe->corpo.x, foe->corpo.y, foe->corpo.z,
               foe->atribs.espera, foe->atribs.cooldown, 100 * foe->precisao,
               foe->atribs.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("     ( x, y, z)            [ vx, vy, vz]         Amigo? ");
    puts("-------------------    --------------------     --------");
    for (Celula *p = getListaProjeteis(); p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%4.0f, %3.0f, %4.0f)      [%4.1f, %4.1f, %5.1f]        %s\n",
               bullet->corpo.x, bullet->corpo.y, bullet->corpo.z,
               bullet->vx, bullet->vy, bullet->vz,
               bullet->amigo ? "sim" : "não");
    }
}
