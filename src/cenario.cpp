#include <cstdio>    /* puts, printf, system */
#include <cstdlib>   /* exit */
#include <cmath>     /* sin */
#include <GL/freeglut.h>

#include "cenario.hpp"
#include "nave.hpp"
#include "defesa.hpp"
#include "tiro.hpp"
#include "item.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "grafico.hpp"

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

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaCenario(bool godMode, bool _debug)
{
    /* Carrega listas e modelos */
    new Nave(godMode);
    carregaInimigos();

    /* Texturas do cenário */
    carregaTextura("water.ppm", true, &modeloRio);
    carregaTextura("brick.ppm", true, &modeloParede);
    carregaTextura("space.ppm", true, &modeloFundo);
    
    debug = _debug;
}

/*------------------------------------------------------------------*/

void controlaTempo(int unused)
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
        if (not estaPausado()) atualizaCenario();
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

static void imprimeElementos();

void atualizaCenario()
{
    static int contFoe  = TEMPO_INIMIGOS;
    static int contItem = TEMPO_ITEM;

    /* Reconhecimento do teclado */
    keyOperations();
    keySpecialOperations();

    /* Ações relacionadas à nave */
    Nave *nave = getNave();
    nave->move();
    if (nave->invencibilidade > 0) nave->invencibilidade--;

    /* Loop para tratar de inimigos */
    std::vector<Inimigo> *inimigos = getListaInimigos();
    for (size_t i = 0; i < inimigos->size();) {
        Inimigo *foe = &(*inimigos)[i];
        if (nave->colidiuCom(foe)) {
            nave->danifica(foe->danoColisao);
        }
        if (--foe->espera == 0) foe->dispara(nave);
        if (foe->saiu()) {
            inimigos->erase(inimigos->begin() + i);
        } else i++;
    }
    /* Loop para verificar estado dos projéteis */
    std::vector<Projetil> *projeteis = getListaProjeteis();
    for (size_t i = 0; i < projeteis->size();) {
        Projetil *bullet = &(*projeteis)[i];
        bullet->move();
        if (bullet->verificaAcerto() or bullet->saiu()) {
            projeteis->erase(projeteis->begin() + i);
        } else i++;
    }
     /* Loop para tratar de itens */
    std::vector<Item> *itens = getListaItens();
    for (size_t i = 0; i < itens->size();) {
        Item *item = &(*itens)[i];
        if (nave->colidiuCom(item)) {
            item->ativa();
            itens->erase(itens->begin() + i);
        }
        else if (item->saiu()) {
            itens->erase(itens->begin() + i);
        }
        else i++;
    }
    /* Gera inimigo ou item se contador chegar a zero */
    if (--contFoe <= 0) {
        new Inimigo(nave->z + Z_DIST);
        contFoe = TEMPO_INIMIGOS;
    }
    if (--contItem <= 0) {
        new Item(nave->z + Z_DIST);
        contItem = TEMPO_ITEM;
    }
    if (debug) imprimeElementos();

    /* Se acabaram vidas, encerra o jogo */
    if (nave->vidas <= 0) encerraJogo();
}

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

    Nave *nave = (Nave *) getNave();
    puts("{Nave}");
    printf("PONTUAÇÂO: %d\n", nave->score);
    printf("VIDAS: %d\n", nave->vidas);
    printf("Energia: %-3d/%d\n", nave->hp, NAVE_HPMAX);
    printf("Posição: (%.0f, %.0f, %.0f)\n", 
           nave->x, nave->y, nave->z);
    printf("Ângulos: (%.0f°, %.0f°)\n",
           180/PI * nave->angHoriz, 180/PI * nave->angVert);
    
    puts("\n{Inimigos}");
    puts("    ( x, y, z)          Recarga    Precisão    Energia ");
    puts("-------------------     -------    --------   ---------");
    for (Inimigo foe : *getListaInimigos()) {
        printf(" (%4.0f, %3.0f, %4.0f)       "
               "%2d/%3d       %3.0f%%       %2d/%2d\n",
               foe.x, foe.y, foe.z,
               foe.espera, foe.cooldown, 100 * foe.precisao,
               foe.hp, FOE_HPMAX);
    }
    puts("\n{Projéteis}");
    puts("     ( x, y, z)            [ vx, vy, vz]         Amigo? ");
    puts("-------------------    --------------------     --------");
    for (Projetil bullet : *getListaProjeteis()) {
        printf(" (%4.0f, %3.0f, %4.0f)      [%4.1f, %4.1f, %5.1f]        %s\n",
               bullet.x,  bullet.y,  bullet.z,
               bullet.vx, bullet.vy, bullet.vz,
               bullet.amigo ? "sim" : "não");
    }
}

/*------------------------------------------------------------------*/

static void desenhaFundo();
static void desenhaRio();
static void desenhaParede();
static void desenhaSuperficie(GLuint texture, GLdouble coords[4][2],
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
    for (Inimigo foe : *getListaInimigos()) {
        foe.desenha();
    }
    for (Projetil bullet : *getListaProjeteis()) {
        bullet.desenha();
    }
    for (Item item : *getListaItens()) {
        item.desenha();
    }
    getNave()->desenha();

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
    GLdouble z = getNave()->z/768.0;  /* 512 + 256 */

    GLdouble coords[4][2] = {
        { 0.0, 4.0 + z }, { 4.0, 4.0 + z },
        { 4.0,     + z }, { 0.0,     + z }
    };
    GLdouble vertex[4][3] = {
        { -X_MAX, 0.0, DIST_CAMERA + Z_DIST },
        {  X_MAX, 0.0, DIST_CAMERA + Z_DIST },
        {  X_MAX, 0.0,                  0.0 },
        { -X_MAX, 0.0,                  0.0 }
    };
    desenhaSuperficie(modeloRio.texturaId, coords, vertex);
}

/*
 *  Desenha as paredes que limitam lateralmente o jogo, 
 *  atribuindo-lhes uma textura e também produzindo movimento.
 */
static void desenhaParede()
{
    GLdouble z = getNave()->z/192.0;  /* 128 + 64 */

    GLdouble coords[4][2] = {
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
    desenhaSuperficie(modeloParede.texturaId, coords, vertex);

    /* Troca o sinal das coordenadas x, para desenhar a outra parede */
    for (int i = 0; i < 4; i++) {
        vertex[i][0] *= -1;
    }
    desenhaSuperficie(modeloParede.texturaId, coords, vertex);
}

/*
 *  Desenha o plano de fundo, atribuindo-lhe uma textura.
 */
static void desenhaFundo()
{
    GLdouble coords[4][2] = {
        { 0.0, 1.0 }, { 4.0, 1.0 },
        { 4.0, 0.0 }, { 0.0, 0.0 }
    };
    GLdouble vertex[4][3] = {
        { -35 * X_MAX, 20 * Y_MAX, 0.0 },
        {  35 * X_MAX, 20 * Y_MAX, 0.0 },
        {  35 * X_MAX,        0.0, 0.0 },
        { -35 * X_MAX,        0.0, 0.0 }
    };
    desenhaSuperficie(modeloFundo.texturaId, coords, vertex);
}

/*
 *  Faz o desenho de uma superfície quadrilateral na tela.
 *    - vertices: indica os vértices da superfície;
 *    - texturaId: inteiro representando a textura;
 *    - coords: coordenadas da textura.
 */
static void desenhaSuperficie(GLuint texturaId, GLdouble coords[4][2],
                              GLdouble vertices[4][3])
{
    glPushMatrix();
    glTranslated(0.0, 0.0, getNave()->z - DIST_CAMERA);
    glBindTexture(GL_TEXTURE_2D, texturaId);

    glBegin(GL_QUADS); 
    for (int i = 0; i < 4; i++) {
        glTexCoord2dv(coords[i]);
        glVertex3dv(vertices[i]);
    } 
    glEnd();

    glPopMatrix();
}

/*------------------------------------------------------------------*/

void encerraJogo()
{
    int score = getNave()->score;

    /* Libera elementos do jogo */
    liberaNave();
    liberaInimigos();

    /* Libera texturas do cenário */
    liberaTextura(&modeloRio);
    liberaTextura(&modeloParede);
    liberaTextura(&modeloFundo);

    /* Mostra score e dá adeus */
    printf("Score final: %d\n", score);
    exit(EXIT_SUCCESS);
}
