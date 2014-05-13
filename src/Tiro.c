#include <math.h>  /* sqrt, abs, sin, cos, atan */
#include "Nave.h"
#include "Tiro.h"
#include "Cenario.h"
#include "Base.h"
#include "Random.h"

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
bool projetilAcertou(Projetil *bullet)
{
    /* ALTERAR PARA FUNCIONAR COM OS TIROS DA NAVE NOS INIMIGOS TAMBÈM !!! */
    int dx = bullet->x - nave.x;
    int dy = bullet->y - nave.y;
    int dz = bullet->z - nave.z;
    
    double d = sqrt(sq(dx) + sq(dy) + sq(dz));

    if (d < (NAVE_RAIO + BALA_RAIO)) return true;
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
    return (bullet->z < nave.z - NAVE_RAIO)
        || (bullet->y <= 0 || bullet->y >= Y_MAX)
        || (abs(bullet->x) >= X_MAX);
}
