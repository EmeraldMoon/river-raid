#include <math.h>  /* abs, sin, cos, atan2 */

#include "Tiro.h"
#include "Base.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Random.h" 

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaProjetil(Projetil *bullet)
{
    listaInsere(projeteis, bullet);
}

/*------------------------------------------------------------------*/

static void calculaAngulo(double *a, double *b, double desvio);

/*
 *  
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 */
void aplicaPrecisao(Projetil *bullet, double precisao)
{
    double *vx = &(bullet->vx);
    double *vy = &(bullet->vy);
    double *vz = &(bullet->vz);
    double desvio = (1 - precisao) * DESVIO_MAX;    

    calculaAngulo(vx, vz, desvio);  /* desvio horizontal */
    calculaAngulo(vy, vz, desvio);  /* desvio vertical   */
}

/*
 *  Recebe duas componentes, encontra seu ângulo em coordenadas 
 *  polares, modifica-o por meio de uma Normal e atualiza os valores.
 */
static void calculaAngulo(double *a, double *b, double desvio)
{
    double v = hypot(*a, *b);
    double ang = atan2(*b, *a);
    ang = normal(ang, desvio);
    *a = cos(ang) * v;
    *b = sin(ang) * v; 
}

/*------------------------------------------------------------------*/

void moveProjetil(Projetil *bullet)
{
    /* Efeito da gravidade */
    bullet->vy -= ACEL_GRAVIDADE;

    bullet->corpo.x += bullet->vx;
    bullet->corpo.y += bullet->vy;
    bullet->corpo.z += bullet->vz;
}

/*------------------------------------------------------------------*
 *
 *  Considera-se que o projétil acertou um elemento caso a distância
 *  entre ambos seja MENOR que a soma dos raios (d < r + R).
 *
 */
bool verificaAcerto(Projetil *bullet, Nave *nave)
{
    /* Verificação de colisão com a nave */
    if (ocorreuColisao(&bullet->corpo, &nave->corpo)) {
        danificaNave(bullet->dano);
        return true;
    }
    /* Verificação de colisão com algum inimigo */
    for (Celula *p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        if (ocorreuColisao(&bullet->corpo, &foe->corpo)) {
            foe->atribs.hp -= bullet->dano;
            if (bullet->amigo) nave->score += PONTOS_ACERTO;
            if (foe->atribs.hp <= 0) {
                listaRemove(p);
                if (bullet->amigo) nave->score += PONTOS_DESTRUCT;
            }
            return true;
        }
    }
    return false;
}

/*------------------------------------------------------------------*/

void desenhaProjetil(Projetil *bullet)
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslated(bullet->corpo.x, bullet->corpo.y, bullet->corpo.z);

    /* Provisório, no futuro cada tiro terá sua estrutura */
    if (bullet->amigo) glColor(LIGHT_GRAY);
    else               glColor(LIME);
  
    glutSolidSphere(bullet->corpo.raio, SLICES, STACKS);
    glPopMatrix();
}
