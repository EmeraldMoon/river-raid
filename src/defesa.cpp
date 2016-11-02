#include <math.h>  /* sqrt */

#include "Defesa.h"
#include "Random.h"
#include "Tiro.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Modelo.h"
#include "Cores.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de inimigos em jogo */
static Lista *inimigos;

/* Modelo dos inimigos */
static Modelo modelo;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaInimigos()
{
    /* Carrega modelo da nave */
    leVertices("Defesa.vert", &modelo);
    carregaTextura("magma.ppm", false, &modelo);

    inimigos = criaLista();
}

/*------------------------------------------------------------------*/

void geraInimigo(double z)
{
    /* Aloca memória */
    Inimigo *foe = mallocSafe(sizeof *foe);

    /* Posicionamento do corpo */
    geraCorpo(&foe->corpo, z);
    foe->corpo.raio       = FOE_RAIO;
    foe->corpo.altura     = 2 * foe->corpo.y;

    /* Atributos restantes */
    foe->atribs.hp        = FOE_HPMAX;
    foe->atribs.cooldown  = foe->atribs.espera = uniforme(80, 105);
    foe->precisao         = uniforme(0.8, 1.0);
    foe->danoColisao      = DANO_COLISAO;
    foe->pontosAcerto     = PONTOS_ACERTO;
    foe->pontosDestruicao = PONTOS_DESTRUICAO;
    foe->tempoDano        = 0;

    listaInsere(inimigos, foe);
}

/*------------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe, Nave *nave)
{
    /* Aloca espaço para o tiro disparado */
    Projetil *bullet = criaProjetil();
    bullet->amigo = false;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    double dx = nave->corpo.x - foe->corpo.x;
    double dy = nave->corpo.y - foe->corpo.y;
    double dz = (nave->corpo.z + nave->corpo.raio) - foe->corpo.z;
    double d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    double k = bullet->dano/d;
    bullet->vx = k * dx;
    bullet->vy = k * dy;
    bullet->vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    double r = (foe->corpo.raio + bullet->corpo.raio)/d;
    bullet->corpo.x = foe->corpo.x + (r * dx);
    bullet->corpo.y = foe->corpo.y + (r * dy);
    bullet->corpo.z = foe->corpo.z + (r * dz);

    /* Aplica desvio de precisão */    
    aplicaPrecisao(bullet, foe->precisao);

    /* Reinicia contagem até próximo tiro */
    foe->atribs.espera = foe->atribs.cooldown;
}

/*------------------------------------------------------------------*/

void desenhaInimigo(Inimigo *foe)
{
    if (foe->tempoDano > 0) foe->tempoDano--;

    glPushMatrix();
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    /* Aplica cor e textura */
    setColor(WHITE);
    glBindTexture(GL_TEXTURE_2D,
                  (foe->tempoDano == 0) ? modelo.texturaId : 0);

    /* Posiciona e dimensiona inimigo */
    glTranslated(foe->corpo.x, 0.0, foe->corpo.z);
    glScaled(16 * foe->corpo.raio, foe->corpo.altura, 16 * foe->corpo.raio);

    /* Desenha modelo baseado em vértices e normais */
    glVertexPointer(3, GL_DOUBLE, 0, modelo.coords);
    glDrawArrays(GL_TRIANGLES, 0, modelo.numVertices);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Lista *getListaInimigos()
{
    return inimigos;
}

/*------------------------------------------------------------------*/

void liberaInimigos()
{
    liberaTextura(&modelo);
    liberaVertices(&modelo);
    liberaLista(inimigos);
}
