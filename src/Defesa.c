#include <math.h>  /* sqrt */

#include "Defesa.h"
#include "Random.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Modelo.h"
#include "Textura.h"

/*------------------------------------------------------------------*/

/* Matrizes representando vértices e normais do inimigo */
static GLdouble defesaVertices[DEFESA_NUM_VERTICES][3];
static GLdouble defesaNormais[DEFESA_NUM_NORMAIS][3];

/* Lista de inimigos em jogo */
static Lista *inimigos;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaInimigos()
{
    leVetores(defesaVertices, DEFESA_NUM_VERTICES, DEFESA_MODELO_VERTICES);
    leVetores(defesaNormais,  DEFESA_NUM_NORMAIS,  DEFESA_MODELO_NORMAIS);
    inimigos = criaLista();
}

/*------------------------------------------------------------------*/

void geraInimigo(double z)
{
    /* Aloca memória */
    Inimigo *foe = mallocSafe(sizeof *foe);

    posicionaCorpo(&foe->corpo, z);
    
    foe->corpo.raio       = FOE_RAIO;
    foe->corpo.altura     = 2 * foe->corpo.y;
    foe->atribs.hp        = FOE_HPMAX;
    foe->atribs.cooldown  = foe->atribs.espera = uniforme(80, 105);
    foe->precisao         = uniforme(0.8, 1.0);
    foe->danoColisao      = DANO_COLISAO;
    foe->pontosAcerto     = PONTOS_ACERTO;
    foe->pontosDestruicao = PONTOS_DESTRUICAO;

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
    double k = BALA_VEL/d;
    bullet->vx = k * dx;
    bullet->vy = k * dy;
    bullet->vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    double r = (foe->corpo.raio + BALA_RAIO)/d;
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
    glPushMatrix();
    glColorAlpha(WHITE, 255);
    glTranslated(foe->corpo.x, 0.0, foe->corpo.z);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, defesaTextura);
    /*glRotated(-90.0, 1.0, 0.0, 0.0);*/
    /*glutWireCylinder(foe->corpo.raio, foe->corpo.altura, SLICES, STACKS);*/
    glScaled(400, 1.2*foe->corpo.altura, 400);

    glEnableClientState(GL_VERTEX_ARRAY);

    /* Desenha os vértices do Defesa.ogl */
    glVertexPointer(3, GL_DOUBLE, 0, defesaVertices);
    glNormalPointer(GL_DOUBLE, 0, defesaNormais);
    glDrawArrays(GL_TRIANGLES, 0, DEFESA_NUM_VERTICES);
    
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Lista *getListaInimigos()
{
    return inimigos;
}
