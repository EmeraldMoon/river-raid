#include <cmath>  /* sqrt */

#include "defesa.hpp"
#include "random.hpp"
#include "tiro.hpp"
#include "cenario.hpp"
#include "grafico.hpp"
#include "modelo.hpp"
#include "cores.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Lista de inimigos em jogo */
static std::vector<Inimigo> inimigos;

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
}

/*------------------------------------------------------------------*/

void geraInimigo(double z)
{
    Inimigo foe;

    /* Posicionamento do corpo */
    geraCorpo(&foe, z);
    foe.raio   = FOE_RAIO;
    foe.altura = 2 * foe.y;

    /* Atributos restantes */
    foe.hp               = FOE_HPMAX;
    foe.cooldown         = foe.espera = uniforme(80, 105);
    foe.precisao         = uniforme(0.8, 1.0);
    foe.danoColisao      = DANO_COLISAO;
    foe.pontosAcerto     = PONTOS_ACERTO;
    foe.pontosDestruicao = PONTOS_DESTRUICAO;
    foe.tempoDano        = 0;
    
    inimigos.push_back(foe);
}

/*------------------------------------------------------------------*/

void inimigoDispara(Inimigo *foe, Nave *nave)
{
    Projetil bullet;
    bullet.amigo = false;
    bullet.raio = BALA_RAIO;
    bullet.dano = BALA_DANO;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    double dx = nave->x - foe->x;
    double dy = nave->y - foe->y;
    double dz = (nave->z + nave->raio) - foe->z;
    double d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    double k = bullet.dano/d;
    bullet.vx = k * dx;
    bullet.vy = k * dy;
    bullet.vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    double r = (foe->raio + bullet.raio)/d;
    bullet.x = foe->x + (r * dx);
    bullet.y = foe->y + (r * dy);
    bullet.z = foe->z + (r * dz);

    /* Aplica desvio de precisão */    
    aplicaPrecisao(&bullet, foe->precisao);

    /* Coloca o projétil em jogo */
    criaProjetil(&bullet);

    /* Reinicia contagem até próximo tiro */
    foe->espera = foe->cooldown;
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
    glTranslated(foe->x, 0.0, foe->z);
    glScaled(16 * foe->raio, foe->altura, 16 * foe->raio);

    /* Desenha modelo baseado em vértices e normais */
    glVertexPointer(3, GL_DOUBLE, 0, modelo.coords);
    glDrawArrays(GL_TRIANGLES, 0, modelo.numVertices);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();
}

/*------------------------------------------------------------------*/

std::vector<Inimigo> *getListaInimigos()
{
    return &inimigos;
}

/*------------------------------------------------------------------*/

void liberaInimigos()
{
    liberaTextura(&modelo);
    liberaVertices(&modelo);
}
