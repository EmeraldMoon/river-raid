#include <math.h>  /* sqrt */

#include "Random.h"
#include "Defesa.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Modelo.h"
#include "Textura.h"

/* Matrizes representando vértices e normais do inimigo */
static GLdouble defesaVertices[DEFESA_NUM_VERTICES][3];
static GLdouble defesaNormais[DEFESA_NUM_NORMAIS][3];

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void carregaModeloInimigos()
{
    leVetores(defesaVertices, DEFESA_NUM_VERTICES, DEFESA_MODELO_VERTICES);
    leVetores(defesaNormais,  DEFESA_NUM_NORMAIS,  DEFESA_MODELO_NORMAIS);
}

/*------------------------------------------------------------------*/

void criaInimigo(Inimigo *foe)
{
    listaInsere(inimigos, foe);
}

/*------------------------------------------------------------------*/

void geraInimigo(double z)
{
    Inimigo *foe = mallocSafe(sizeof *foe);

    foe->corpo.x = (X_MAX - FOE_RAIO) * uniformeDouble(-1.0, 1.0);
    foe->corpo.y = uniformeInt(Y_MAX/8, Y_MAX/2);
    foe->corpo.z = z;

    foe->atribs.hp       = FOE_HPMAX;
    foe->atribs.cooldown = uniformeInt(80, 105);
    foe->atribs.espera   = foe->atribs.cooldown;
    foe->corpo.raio     = FOE_RAIO;
    foe->corpo.altura   = 2 * foe->corpo.y;
    foe->precisao      = uniformeDouble(0.8, 1.0);

    criaInimigo(foe);
}

/*------------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe, Nave *nave)
{
    Projetil *bullet = mallocSafe(sizeof *bullet);
    int dx, dy, dz;
    double d, k, r;

    bullet->dano       = BALA_DANO;
    bullet->corpo.raio = BALA_RAIO;
    bullet->amigo = false;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    dx = nave->corpo.x - foe->corpo.x;
    dy = nave->corpo.y - foe->corpo.y;
    dz = (nave->corpo.z + nave->corpo.raio) - foe->corpo.z;
    d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    k = BALA_VEL/d;
    bullet->vx = k * dx;
    bullet->vy = k * dy;
    bullet->vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    r = (foe->corpo.raio + BALA_RAIO)/d;
    bullet->corpo.x = foe->corpo.x + (r * dx);
    bullet->corpo.y = foe->corpo.y + (r * dy);
    bullet->corpo.z = foe->corpo.z + (r * dz);
    
    aplicaPrecisao(bullet, foe->precisao);
    criaProjetil(bullet);

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
