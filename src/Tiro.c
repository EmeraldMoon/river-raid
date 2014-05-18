#include <math.h>  /* sqrt, abs, sin, cos, atan */
#include "Tiro.h"
#include "Nave.h"
#include "Defesa.h"
#include "Cenario.h"
#include "Base.h"
#include "Random.h"

static bool projetilColidiu(Projetil *bullet, Esqueleto corpo);

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaProjetil(Projetil bullet)
{
    insere(projeteis, &bullet, sizeof bullet);
}

/*------------------------------------------------------------*
 *  
 *  Os desvios são calculados segundo uma distribuição Normal.
 *  Enquanto a trajetória é alterada, o módulo da velocidade
 *  permanece constante.
 *
 *  Observe que o desvio mínimo é 0° e o máximo 90°.
 *
 */
void aplicaPrecisao(Projetil *bullet, double precisao)
{
    double ang, hip;
    double desvio = (1 - precisao) * PI/2;

    double vx = bullet->vx;
    double vy = bullet->vy;
    double vz = bullet->vz;

    /* Desvio horizontal */
    ang = normal(atan(vx/vz), desvio);
    hip = sqrt(sq(vx) + sq(vz));
    vx = -sin(ang) * hip;
    vz = -cos(ang) * hip;

    /* Desvio vertical */
    ang = normal(atan(vy/vz), desvio);
    hip = sqrt(sq(vy) + sq(vz));
    vy = -sin(ang) * hip;
    vz = -cos(ang) * hip;
}

/*------------------------------------------------------------*/

void moveProjetil(Projetil *bullet)
{
    /* Efeito da gravidade */
    bullet->vy -= ACEL_GRAV;

    bullet->x += bullet->vx;
    bullet->y += bullet->vy;
    bullet->z += bullet->vz;
}

/*------------------------------------------------------------*
 *
 *  Para efeito de testes, considera-se que o projétil
 *  acertou um elemento caso a distância entre os dois 
 *  seja MENOR que a soma dos raios de ambos (d < r + R).
 *
 */
Alvo projetilAcertou(Projetil *bullet)
{
    Celula *p;

    /* Verificação de colisão com a nave */
    if (projetilColidiu(bullet, nave.base)) return NAVE;

    /* Verificação de colisão com algum inimigo */
    for (p = inimigos; p->prox != NULL; p = p->prox) {
        Inimigo *foe = p->prox->item;
        if (projetilColidiu(bullet, foe->base)) return INIMIGO;
    }

    return MISS;
}

static bool projetilColidiu(Projetil *bullet, Esqueleto corpo)
{
    double d, dAltura;
    int dx = bullet->x - corpo.x;
    int dy = bullet->y - corpo.y;
    int dz = bullet->z - corpo.z;
    int somaRaios = corpo.raio + BALA_RAIO;

    /* Esta parte visa evitar cálculos desnecessários */
    if (dx >= somaRaios || dz >= somaRaios) return false;

    d = sqrt(sq(dx) + sq(dy) + sq(dz));
    dAltura = abs(dy) - (corpo.altura/2 + corpo.raio);

    return (d < somaRaios && dAltura > 0);
}

/*------------------------------------------------------------*
 *
 *  O projétil passou INTEIRAMENTE pela nave?
 *  O projétil saiu pelo limite superior ou inferior?
 *  O projétil saiu por um dos lados da tela?
 *  Caso uma das respostas seja sim, o projétil saiu do jogo.
 *
 */
bool projetilSaiu(Projetil *bullet)
{
    return (bullet->z < nave.base.z - nave.base.raio)
        || (bullet->y <= 0 || bullet->y >= Y_MAX)
        || (abs(bullet->x) >= X_MAX);
}
