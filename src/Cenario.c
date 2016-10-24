#include <stdio.h>   /* puts, printf, system */
#include <stdlib.h>  /* exit */

#include "Cenario.h"
#include "Teclado.h"
#include "Textura.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

/* Elementos básicos do jogo */
Nave *nave;
Lista *inimigos;
Lista *projeteis;
Lista *items;

/* Variáveis globais de tempo */
GLuint dt, t0 = 0;

static void imprimeElementos();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void inicializaCenario(bool godMode)
{
    carregaNave(godMode);
    inimigos  = criaLista();
    projeteis = criaLista();
    items = criaLista();
}

/*------------------------------------------------------------------*/

void tempo()
{
    const GLint INTERVALO = 1000/FPS;
    static int tExtra;

    /* Obtém tempo desde última atualização */
    dt = glutGet(GLUT_ELAPSED_TIME) - t0;

    /* Limita FPS para programa não ir rápido demais */
    if (dt < INTERVALO) {
        glutTimerFunc(INTERVALO - dt, tempo, 0);
        return;
    }

    /* Acumula tempo extra gasto para desenhar */
    tExtra += dt - INTERVALO;

    /* Caso tempo acumulado chegue a um ou mais frames inteiros, 
       faz a interpolação deles entre o anterior e o próximo desenho. */
    for (;;) {
        atualiza();
        if (tExtra < 2 * INTERVALO) break;
        tExtra -= INTERVALO;
    }

    t0 = glutGet(GLUT_ELAPSED_TIME);

    /* Chama a função de desenho após esta */
    glutPostRedisplay();
}

/*------------------------------------------------------------------*/

void atualiza()
{
    static int contFoe  = TEMPO_INIMIGOS;
    static int contItem = TEMPO_ITEM;

    /* Reconhecimento do teclado */
    keyOperations();
    keySpecialOperations();

    /* Ações relacionadas à nave */
    moveNave();
    if (nave->invencibilidade > 0) (nave->invencibilidade)--;

    /* Loop para tratar de inimigos */
    Celula *p = inimigos;
    while (p->prox != NULL) {
        Inimigo *foe = p->prox->item;
        if (ocorreuColisao(&nave->corpo, &foe->corpo)) danificaNave(DANO_COLISAO);
        if (foe->atribs.espera-- == 0) inimigoDispara(foe, nave);
        if (corpoSaiu(&foe->corpo, nave->corpo.z)) listaRemove(p);
        else p = p->prox;
    }
    /* Loop para tratar de itens */
    Celula *q = items;
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
    p = projeteis;
    while (p->prox != NULL) {
        Projetil *bullet = p->prox->item;
        moveProjetil(bullet);
        if (verificaAcerto(bullet, nave) || corpoSaiu(&bullet->corpo, nave->corpo.z)) listaRemove(p);
        else p = p->prox;
    }
    if (nave->vidas <= 0) encerraJogo();
    if (--contFoe == 0) {
        geraInimigo();
        contFoe = TEMPO_INIMIGOS;
    }
    if (--contItem == 0) {
        geraItem();
        contItem = TEMPO_ITEM;
    }
    /*imprimeElementos();*/
}

/*------------------------------------------------------------------*/

void encerraJogo()
{
    liberaLista(inimigos);
    liberaLista(projeteis);
    liberaLista(items);
    liberaTexturas();

    printf("Score final: %d\n", nave->score);
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
    printf("PONTUAÇÂO: %d\n", nave->score);
    printf("VIDAS: %d\n", nave->vidas);
    printf("Energia: %-3d/%d\n", nave->atribs.hp, NAVE_HPMAX);
    printf("Posição: (%.0f, %.0f, %.0f)\n", 
        nave->corpo.x, nave->corpo.y, nave->corpo.z);
    printf("Ângulos: (%.0f°, %.0f°)\n",
        (180/PI) * nave->angHoriz, (180/PI) * nave->angVert);
    
    /* Para efeitos de clareza, todas as componentes z dos
       inimigos e projéteis são relativas à nave (e não absolutas). */
    puts("\n{Inimigos}");
    puts("    ( x, y, z)          Recarga    Precisão    Energia ");
    puts("-------------------     -------    --------   ---------");
    for (Celula *p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        printf(" (%4g, %3g, %4g)       %2d/%2d       %3.0f%%       %2d/%2d\n",
            foe->corpo.x, foe->corpo.y, (foe->corpo.z - nave->corpo.z),
            foe->atribs.espera, foe->atribs.cooldown, 100 * foe->precisao,
            foe->atribs.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("     ( x, y, z)            [ vx, vy, vz]         Amigo? ");
    puts("-------------------    --------------------     --------");
    for (Celula *p = projeteis; p->prox != NULL; p = p->prox) {
        Projetil *bullet = p->prox->item;
        printf(" (%4.0f, %3.0f, %4.0f)      [%4.1f, %4.1f, %5.1f]        %s\n",
            bullet->corpo.x, bullet->corpo.y, (bullet->corpo.z - nave->corpo.z),
            bullet->vx, bullet->vy, bullet->vz,
            (bullet->amigo) ? "sim" : "não");
    }
}
