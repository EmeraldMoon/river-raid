#include <cmath>  /* abs, sin, cos, atan2 */

#include "tiro.hpp"
#include "base.hpp"
#include "lista.hpp"
#include "random.hpp"
#include "defesa.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de projéteis no campo de jogo */
static Lista *projeteis;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void carregaProjeteis()
{
    projeteis = criaLista();
}

/*------------------------------------------------------------------*/

Projetil *criaProjetil()
{
    /* Aloca espaço para projétil */
    Projetil *bullet = (    Projetil *) mallocSafe(sizeof *bullet);

    /* Atributos fixos do projétil */
    bullet->dano         = BALA_DANO;
    bullet->corpo.raio   = BALA_RAIO;
    bullet->corpo.altura = 2 * bullet->corpo.raio;

    listaInsere(projeteis, bullet);

    return bullet;
}

/*------------------------------------------------------------------*/

static void calculaAngulo(double *a, double *b, double desvio);

/*
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 */
void aplicaPrecisao(Projetil *bullet, double precisao)
{
    double desvio = (1 - precisao) * DESVIO_MAX;

    calculaAngulo(&bullet->vx, &bullet->vz, desvio);  /* desvio horizontal */
    calculaAngulo(&bullet->vy, &bullet->vz, desvio);  /* desvio vertical   */
}

/*
 *  Recebe duas componentes, encontra seu ângulo em coordenadas 
 *  polares, modifica-o por meio de uma Normal e atualiza os valores.
 */
static void calculaAngulo(double *a, double *b, double desvio)
{
    double   v = hypot(*a, *b);
    double ang = atan2(*b, *a);
    ang = normal(ang, desvio);
    *a = v * cos(ang);
    *b = v * sin(ang); 
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

/*------------------------------------------------------------------*/

/*
 *  Considera-se que o projétil acertou um elemento caso a distância
 *  entre ambos seja MENOR que a soma dos raios (d < r + R).
 */
bool verificaAcerto(Projetil *bullet)
{
    Nave *nave = getNave();

    /* Verificação de colisão com a nave */
    if (ocorreuColisao(&bullet->corpo, &nave->corpo)) {
        danificaNave(bullet->dano);
        return true;
    }
    /* Verificação de colisão com algum inimigo */
    for (Celula *p = getListaInimigos(); p->prox != NULL; p = p->prox) {
        Inimigo *foe = (Inimigo *) p->prox->item;
        if (!ocorreuColisao(&bullet->corpo, &foe->corpo)) continue;
        if (bullet->amigo) {
            foe->atribs.hp -= bullet->dano;
            nave->score += foe->pontosAcerto;
            if (foe->atribs.hp <= 0) {
                listaRemoveProx(p);
                nave->score += foe->pontosDestruicao;
            } else {
                foe->tempoDano = FOE_TEMPO_DANO;
            }
        }
        return true;
    }
    return false;
}

/*------------------------------------------------------------------*/

void desenhaProjetil(Projetil *bullet)
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Cores diferentes para tiros diferentes */
    if (bullet->amigo) setColor(LIGHT_GRAY);
    else               setColor(YELLOW);
  
    /* Desenha esfera na posição (x, y, z) */
    glTranslated(bullet->corpo.x, bullet->corpo.y, bullet->corpo.z);
    glutSolidSphere(bullet->corpo.raio, SLICES, STACKS);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

Lista *getListaProjeteis()
{
    return projeteis;
}

/*------------------------------------------------------------------*/

void liberaProjeteis()
{
    liberaLista(projeteis);
}
