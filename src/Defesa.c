#include <math.h>  /* sqrt */
#include "Defesa.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Random.h"
#include "Textura.h"

/* Vértices do modelo das torres */
#include "Defesa.ogl"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaInimigo(Inimigo foe)
{
    insere(inimigos, &foe, sizeof foe);
}

/*------------------------------------------------------------------*/

void geraInimigo()
{
    Inimigo foe;

    /* (uniforme(-1, 0) | 1) gera um número que é 1 ou -1 */
    foe.base.x = (X_MAX - FOE_RAIO) * (uniforme(-1, 0) | 1);
    foe.base.y = uniforme(Y_MAX/8, Y_MAX/2);
    foe.base.z = nave.base.z + Z_MAX;

    foe.base.hp       = FOE_HPMAX;
    foe.base.cooldown = uniforme(45, 60);
    foe.base.espera   = foe.base.cooldown;
    foe.base.raio     = FOE_RAIO;
    foe.base.altura   = 2 * foe.base.y;
    foe.precisao      = uniformeD(0.8, 1.0);

    criaInimigo(foe);
}

/*------------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe)
{
    Projetil bullet;
    int dx, dy, dz;
    double d, k, r;

    bullet.dano = BALA_DANO;
    bullet.raio = BALA_RAIO;
    bullet.amigo = false;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    dx = nave.base.x - foe->base.x;
    dy = nave.base.y - foe->base.y;
    dz = (nave.base.z + nave.base.raio) - foe->base.z;
    d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    k = BALA_VEL/d;
    bullet.vx = k * dx;
    bullet.vy = k * dy;
    bullet.vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    r = (foe->base.raio + BALA_RAIO)/d;
    bullet.x = foe->base.x + (r * dx);
    bullet.y = foe->base.y + (r * dy);
    bullet.z = foe->base.z + (r * dz);
    
    aplicaPrecisao(&bullet, foe->precisao);
    criaProjetil(bullet);

    /* Reinicia contagem até próximo tiro */
    foe->base.espera = foe->base.cooldown;
}

/*------------------------------------------------------------------*/

bool inimigoSaiu(Inimigo *foe)
{
    return (foe->base.z < nave.base.z - DIST_CAMERA);
}

/*------------------------------------------------------------------*/

void desenhaInimigo(Inimigo *foe)
{
    glPushMatrix();
    glColorAlpha(WHITE, 255);
    glTranslated(foe->base.x, 0.0, foe->base.z);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, defesaTextura);
    /*glRotated(-90.0, 1.0, 0.0, 0.0);*/
    /*glutWireCylinder(foe->base.raio, foe->base.altura, SLICES, STACKS);*/
    glScaled(400, 1.2*foe->base.altura, 400);

    glEnableClientState(GL_VERTEX_ARRAY);

    /* Desenha os vértices do Defesa.ogl */
    glVertexPointer(3, GL_FLOAT, 0, towerVerts);
    glNormalPointer(GL_FLOAT, 0, towerNormals);
    glDrawArrays(GL_TRIANGLES, 0, towerNumVerts);
    
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();
}
