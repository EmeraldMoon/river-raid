#include <stdio.h>    /* puts, printf, system */
#include <stdlib.h>   /* exit */
#include <stdbool.h>  /* bool */
#include <math.h>     /* sin */
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
static int dt, t0 = 0;

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
    static int tExtra = 0;

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

static void desenhaFundo();
static void desenhaRio();
static void desenhaParede();

void desenhaCenario()
{
    /* Ativa opções para desenho de objetos */
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    /* Desenha elementos estáticos do cenário */
    desenhaRio();
    desenhaParede();
    desenhaFundo();
    
    /* Desenha elementos dinâmicos do jogo */
    for (Celula *p = getListaItens()->prox; p != NULL; p = p->prox) {
        Item *item = p->item;
        desenhaItem(item);
    }
    for (Celula *p = getListaInimigos()->prox; p != NULL; p = p->prox) {
        Inimigo *foe = p->item;
        desenhaInimigo(foe);
    }
    for (Celula *p = getListaProjeteis()->prox; p != NULL; p = p->prox) {
        Projetil *bullet = p->item;
        desenhaProjetil(bullet);
    }
    desenhaNave();

    /* Desativa opções para não prejudicar desenho de hud e etc */
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}

/*
 *  Desenha o "chão" do cenário, o limite inferior do jogo.
 *  Simula uma sensação de movimento com o correr do rio.
 */
static void desenhaRio()
{
    GLdouble t = (t0 % 10000) / 10;
    const GLdouble DIST_RIO = X_MAX + getNave()->corpo.raio * sin(ANG_MAX);

    glPushMatrix();
    glTranslated(0.0, 0.0, getNave()->corpo.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, rioTextura);

    const double coord[4][2] = {
        { 0, 8 + t/128 }, { 4, 8 + t/128 },
        { 4,     t/128 }, { 0,     t/128 }
    };

    const int vertex[4][3] = {
        { -DIST_RIO, 0, Z_DIST + DIST_CAMERA },
        {  DIST_RIO, 0, Z_DIST + DIST_CAMERA },
        {  DIST_RIO, 0,                   0 },
        { -DIST_RIO, 0,                   0 }
    };

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coord[i]);
        glVertex3iv(vertex[i]);
    } 
    glEnd();

    glPopMatrix();
}

/*
 *  Desenha as paredes que limitam lateralmente o jogo, 
 *  atribuindo-lhes uma textura e também produzindo movimento.
 */
static void desenhaParede()
{
    const GLdouble DIST_PAREDE = X_MAX + getNave()->corpo.raio * sin(ANG_MAX);
    GLdouble t = (t0 % 10000) / 10;
    
    glPushMatrix();
    glTranslated(0, 0, getNave()->corpo.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, paredeTextura);

    const double coords[4][2] = {
        {      t/128, 1 },
        { 16 + t/128, 1 },
        { 16 + t/128, 0 },
        {      t/128, 0 }
    };

    const int verticesFFLCH[4][3] = {
        { -DIST_PAREDE, Y_MAX, 0 },
        { -DIST_PAREDE, Y_MAX, Z_DIST + DIST_CAMERA },
        { -DIST_PAREDE, 0, Z_DIST + DIST_CAMERA },
        { -DIST_PAREDE, 0, 0 }
    };

    /* Parede esquerda */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coords[i]);
        glVertex3iv(verticesFFLCH[i]);
    }
    glEnd();

    const int verticesPOLI[4][3] = {
        { DIST_PAREDE, Y_MAX, 0 },
        { DIST_PAREDE, Y_MAX, Z_DIST + DIST_CAMERA },
        { DIST_PAREDE, 0, Z_DIST + DIST_CAMERA },
        { DIST_PAREDE, 0, 0 }
    };

    /* Parede direita */
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coords[i]);
        glVertex3iv(verticesPOLI[i]);
    }
    glEnd();

    glPopMatrix();
}

/*
 *  Desenha o plano de fundo, atribuindo-lhe uma textura.
 */
static void desenhaFundo()
{
    glPushMatrix();
    glTranslated(0.0, 0.0, getNave()->corpo.z + Z_DIST);
    glBindTexture(GL_TEXTURE_2D, fundoTextura);

    const double coord[4][2] = {
        { 0.0, 1.0 }, { 4.0, 1.0 },
        { 4.0, 0.0 }, { 0.0, 0.0 }
    };

    const int vertex[4][3] = {
        { -35*X_MAX, 20*Y_MAX, 0.0 },
        {  35*X_MAX, 20*Y_MAX, 0.0 },
        {  35*X_MAX,      0.0, 0.0 },
        { -35*X_MAX,      0.0, 0.0 }
    };

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coord[i]);
        glVertex3iv(vertex[i]);
    } 
    glEnd();

    glPopMatrix();
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
