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
    leVertices("defesa.vert", &modelo);
    carregaTextura("magma.ppm", false, &modelo);
}

/*------------------------------------------------------------------*/

Inimigo::Inimigo(double z) : Unidade(z)
{
    /* Posicionamento do corpo */
    this->raio   = FOE_RAIO;
    this->altura = 2 * this->y;

    /* Atributos restantes */
    this->hp               = FOE_HPMAX;
    this->cooldown         = this->espera = uniforme(80, 105);
    this->precisao         = uniforme(0.8, 1.0);
    this->danoColisao      = DANO_COLISAO;
    this->pontosAcerto     = PONTOS_ACERTO;
    this->pontosDestruicao = PONTOS_DESTRUICAO;
    this->tempoDano        = 0;
    
    inimigos.push_back(*this);
}

/*------------------------------------------------------------------*/

void Inimigo::dispara(Nave *nave)
{
    Projetil bullet(nave->z);
    bullet.amigo  = false;

    /* Calcula distância entre coordenadas de inimigo e nave.
       No caso do eixo z, considera-se a posição um pouco à frente. */
    double dx = nave->x - this->x;
    double dy = nave->y - this->y;
    double dz = (nave->z + nave->raio) - this->z;
    double d = norma(dx, dy, dz);

    /* Gera vetor velocidade na referida direção */
    double k = bullet.dano/d;
    bullet.vx = k * dx;
    bullet.vy = k * dy;
    bullet.vz = k * dz;

    /* Posição inicial de projétil segue direção da nave */
    double r = (this->raio + bullet.raio)/d;
    bullet.x = this->x + (r * dx);
    bullet.y = this->y + (r * dy);
    bullet.z = this->z + (r * dz);

    /* Aplica desvio de precisão */
    bullet.aplicaPrecisao(this->precisao);

    /* Insere projétil na lista */
    getListaProjeteis()->push_back(bullet);

    /* Reinicia contagem até próximo tiro */
    this->espera = this->cooldown;
}

/*------------------------------------------------------------------*/

void Inimigo::desenha()
{
    if (this->tempoDano > 0) this->tempoDano--;

    glPushMatrix();
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    /* Aplica cor e textura */
    setColor(WHITE);
    glBindTexture(GL_TEXTURE_2D,
                  (this->tempoDano == 0) ? modelo.texturaId : 0);

    /* Posiciona e dimensiona inimigo */
    glTranslated(this->x, 0.0, this->z);
    glScaled(16 * this->raio, this->altura, 16 * this->raio);

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
