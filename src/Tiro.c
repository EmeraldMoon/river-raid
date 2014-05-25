#include <math.h>  /* sqrt, abs, sin, cos, atan */
#include "Tiro.h"
#include "Nave.h"
#include "Defesa.h"
#include "Cenario.h"
#include "Random.h"

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
    double ang, hip;
    double vx = bullet->vx;
    double vy = bullet->vy;
    double vz = bullet->vz;
    double desvio = (1 - precisao) * DESVIO_MAX;    

    /* Desvio horizontal */
    ang = normal(atan(vx/vz), desvio);
    hip = hipot(vx, vz);
    vx = sin(abs(ang)) * sinal(vx);
    vz = cos(abs(ang)) * sinal(vz);

    /* Desvio vertical */
    ang = normal(atan(vy/vz), desvio);
    hip = hipot(vy, vz);
    vy = sin(abs(ang)) * sinal(vy);
    vz = cos(abs(ang)) * sinal(vz);
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
    Celula *p;

    /* Verificação de colisão com a nave */
    if (projetilColidiu(bullet, nave.base)) {
        danificaNave(bullet->dano);
        return true;
    }

    /* Verificação de colisão com algum inimigo */
    for (p = inimigos; p->prox != NULL; p = p->prox) {
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
        || (bullet->y <      0 || bullet->y > Y_MAX)
        || (bullet->z < nave.base.z - nave.base.raio
            || bullet->z > nave.base.z + Z_MAX);
}

/*------------------------------------------------------------------*
 *
 *  Verifica se houve colisão entre projétil e corpo cilíndrico.
 *
 */

static bool projetilColidiu(Projetil *bullet, Corpo corpo)
{
    int dx = bullet->x - corpo.x;
    int dy = bullet->y - corpo.y;
    int dz = bullet->z - corpo.z;
    int somaRaios = corpo.raio + bullet->raio;

    /* Esta parte visa a evitar cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    return (norma(dx, 0, dz) < somaRaios)
        && (dy < corpo.altura/2 + bullet->raio);
}
