#include <cmath>  /* abs, sin, cos, atan2 */

#include "tiro.hpp"
#include "base.hpp"
#include "random.hpp"
#include "defesa.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de projéteis no campo de jogo */
std::vector<Projetil> projeteis;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

void criaProjetil(Projetil *bullet)
{
    bullet->altura = 2 * bullet->raio;

    projeteis.push_back(*bullet);
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

    bullet->x += bullet->vx;
    bullet->y += bullet->vy;
    bullet->z += bullet->vz;
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
    if (ocorreuColisao(bullet, nave)) {
        danificaNave(bullet->dano);
        return true;
    }
    /* Verificação de colisão com algum inimigo */
    std::vector<Inimigo> *inimigos = getListaInimigos();
    for (size_t i = 0; i < inimigos->size(); i++) {
        Inimigo *foe = &(*inimigos)[i];
        if (!ocorreuColisao(bullet, foe)) continue;
        if (bullet->amigo) {
            foe->hp -= bullet->dano;
            nave->score += foe->pontosAcerto;
            if (foe->hp <= 0) {
                inimigos->erase(inimigos->begin() + i);
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
    glTranslated(bullet->x, bullet->y, bullet->z);
    glutSolidSphere(bullet->raio, SLICES, STACKS);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

std::vector<Projetil> *getListaProjeteis()
{
    return &projeteis;
}

