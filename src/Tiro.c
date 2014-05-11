#include <math.h>  /* sqrt, abs, sin, cos, atan */
#include "Nave.h"
#include "Tiro.h"
#include "Cenario.h"
#include "Random.h"

/* Pi, nossa constante querida */
#define PI 3.14159265

/* Atalhos para acessar as componentes */
#define vx bullet->vel.x
#define vy bullet->vel.y
#define vz bullet->vel.z

/* Calcula quadrado de x */
#define sq(x) (pow((x), 2))

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaProjetil(Projetil bullet)
{
    insere(projeteis, &bullet, sizeof bullet);
}

/*------------------------------------------------------------*/

void miraProjetil(Projetil *bullet)
{
    int dx = nave.x - bullet->x;
    int dy = nave.y - bullet->y;
    int dz = (nave.z + NAVE_RAIO) - bullet->z;

    /* Fator 'k' é a razão entre velocidade e distância */
    double d = sqrt(sq(dx) + sq(dy) + sq(dz));
    double k = BALA_VEL/d;

    bullet->vel.x = k*dx;
    bullet->vel.y = k*dy;
    bullet->vel.z = k*dz;
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
    bullet->x += bullet->vel.x;
    bullet->y += bullet->vel.y;
    bullet->z += bullet->vel.z;
}

/*------------------------------------------------------------*
 *
 *  Para efeito de testes, considera-se que o projétil
 *  acertou a nave caso a distância entre os dois 
 *  seja MENOR que a soma dos raios de ambos (d < r + R).
 *
 */
bool projetilAcertou(Projetil *bullet)
{
    int dx = bullet->x - nave.x;
    int dy = bullet->y - nave.y;
    int dz = bullet->z - nave.z;
    
    double d = sqrt(sq(dx) + sq(dy) + sq(dz));

    return (d < (NAVE_RAIO + BALA_RAIO));
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
