#include <math.h>  /* abs, sin, cos, atan2 */
#include "Tiro.h"
#include "Cenario.h"
#include "Grafico.h"
#include "Random.h"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

static void calculaAngulo(double *a, double *b, double desvio);
static bool projetilColidiu(Projetil *bullet, Corpo corpo);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaProjetil(Projetil bullet)
{
    insere(projeteis, &bullet, sizeof bullet);
}

/*------------------------------------------------------------------*
 *  
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 *
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
    double v = hipot(*a, *b);
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

    bullet->x += bullet->vx;
    bullet->y += bullet->vy;
    bullet->z += bullet->vz;
}

/*------------------------------------------------------------------*
 *
 *  Considera-se que o projétil acertou um elemento caso a distância
 *  entre ambos seja MENOR que a soma dos raios (d < r + R).
 *
 */
bool verificaAcerto(Projetil *bullet)
{
    /* Verificação de colisão com a nave */
    if (projetilColidiu(bullet, nave.base)) {
        danificaNave(bullet->dano);
        return true;
    }

    /* Verificação de colisão com algum inimigo */
    for (Celula *p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        if (projetilColidiu(bullet, foe->base)) {
            foe->base.hp -= bullet->dano;
            if (bullet->amigo) nave.score += PONTOS_ACERTO;
            if (foe->base.hp <= 0) {
                exclui(p);
                if (bullet->amigo) nave.score += PONTOS_DESTRUCT;
            }
            return true;
        }
    }

    return false;
}

/*
 *  Verifica se há colisão entre projétil e corpo cilíndrico.
 */
static bool projetilColidiu(Projetil *bullet, Corpo corpo)
{
    int dx = bullet->x - corpo.x;
    int dy = bullet->y - corpo.y;
    int dz = bullet->z - corpo.z;
    int somaRaios = corpo.raio + bullet->raio;

    /* Esta parte visa a evitar cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    return (hipot(dx, dz) < somaRaios)
        && (abs(dy) < corpo.altura/2 + bullet->raio);
}

/*------------------------------------------------------------------*
 *
 *  O projétil saiu por um dos limites da tela (x, y ou z)?
 *  O projétil ultrapassou inteiramente a nave?
 *  Caso uma das respostas seja sim, o projétil saiu do jogo.
 *
 */
bool projetilSaiu(Projetil *bullet)
{
    return (bullet->x < -X_MAX || bullet->x > X_MAX)
        || (bullet->y < 0)
        || (bullet->z < nave.base.z - nave.base.raio
            || bullet->z > nave.base.z + Z_MAX);
}

/*------------------------------------------------------------------*/

void desenhaProjetil(Projetil *bullet)
{
    glPushMatrix();
    glTranslated(bullet->x, bullet->y, bullet->z);

    /* Provisório, no futuro cada tiro terá sua estrutura */
    if (bullet->amigo) glColor(LIGHT_GRAY);
    else               glColor(LIME);
  
    glutSolidSphere(bullet->raio, SLICES, STACKS);  
    glPopMatrix();
}
