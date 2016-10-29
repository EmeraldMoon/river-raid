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
#include "Modelo.h"
#include "Grafico.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Modelos do cenário */
static Modelo modeloRio;
static Modelo modeloParede;
static Modelo modeloFundo;

/* Guarda intervalo entre chamadas de controlaTempo() */
static int dt;

/* Indica se serão impressas informações de debug */
static bool debug = false;

static void imprimeElementos();

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaCenario(bool godMode, bool _debug)
{
    /* Carrega listas e modelos */
    carregaNave(godMode);
    carregaInimigos();
    carregaProjeteis();
    carregaItens();

    /* Texturas do cenário */
    carregaTextura("water.ppm", true, &modeloRio);
    carregaTextura("brick.ppm", true, &modeloParede);
    carregaTextura("space.ppm", true, &modeloFundo);
    
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
    p = getListaItens();
    while (p->prox != NULL) {
        Item *item = p->prox->item;
        if (ocorreuColisao(&nave->corpo, &item->corpo)) {
            ativaItem(item, nave);
            listaRemove(p);
        }
        else if (corpoSaiu(&item->corpo, nave->corpo.z)) listaRemove(p);
        else p = p->prox;
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

    printf("Score final: %d\n", getNave()->score);
    exit(EXIT_SUCCESS);
}

/*------------------------------------------------------------------*/

static void desenhaFundo();
static void desenhaRio();
static void desenhaParede();
static void desenhaSuperficie(GLuint texture, GLdouble coord[4][2],
                              GLdouble vertex[4][3]);

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
    GLdouble z = getNave()->corpo.z/768.0;  /* 512 + 256 */

    GLdouble coord[4][2] = {
        { 0.0, 4.0 + z }, { 4.0, 4.0 + z },
        { 4.0,     + z }, { 0.0,     + z }
    };
    GLdouble vertex[4][3] = {
        { -X_MAX, 0.0, DIST_CAMERA + Z_DIST },
        {  X_MAX, 0.0, DIST_CAMERA + Z_DIST },
        {  X_MAX, 0.0,                  0.0 },
        { -X_MAX, 0.0,                  0.0 }
    };
    desenhaSuperficie(modeloRio.texturaId, coord, vertex);
}

/*
 *  Desenha as paredes que limitam lateralmente o jogo, 
 *  atribuindo-lhes uma textura e também produzindo movimento.
 */
static void desenhaParede()
{
    GLdouble z = getNave()->corpo.z/192.0;  /* 128 + 64 */

    GLdouble coord[4][2] = {
        {        z, 1.0 },
        { 16.0 + z, 1.0 },
        { 16.0 + z, 0.0 },
        {        z, 0.0 }
    };
    GLdouble vertex[4][3] = {
        { -X_MAX, Y_MAX, 0.0 },
        { -X_MAX, Y_MAX, DIST_CAMERA + Z_DIST },
        { -X_MAX,   0.0, DIST_CAMERA + Z_DIST },
        { -X_MAX,   0.0, 0.0 }
    };
    desenhaSuperficie(modeloParede.texturaId, coord, vertex);

    /* Troca o sinal das coordenadas x, para desenhar a outra parede */
    for (int i = 0; i < 4; i++) {
        vertex[i][0] *= -1;
    }
    desenhaSuperficie(modeloParede.texturaId, coord, vertex);
}

/*
 *  Desenha o plano de fundo, atribuindo-lhe uma textura.
 */
static void desenhaFundo()
{
    GLdouble coord[4][2] = {
        { 0.0, 1.0 }, { 4.0, 1.0 },
        { 4.0, 0.0 }, { 0.0, 0.0 }
    };
    GLdouble vertex[4][3] = {
        { -35 * X_MAX, 20 * Y_MAX, 0.0 },
        {  35 * X_MAX, 20 * Y_MAX, 0.0 },
        {  35 * X_MAX,        0.0, 0.0 },
        { -35 * X_MAX,        0.0, 0.0 }
    };
    desenhaSuperficie(modeloFundo.texturaId, coord, vertex);
}

/*
 *  Faz o desenho de uma superfície quadrilateral na tela.
 *    - vertices: indica os vértices da superfície;
 *    - texturaId: inteiro representando a textura;
 *    - coord: coordenadas da textura.
 */
static void desenhaSuperficie(GLuint texturaId, GLdouble coord[4][2],
                              GLdouble vertices[4][3])
{
    glPushMatrix();
    glTranslated(0.0, 0.0, getNave()->corpo.z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, texturaId);

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coord[i]);
        glVertex3dv(vertices[i]);
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
