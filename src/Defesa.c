#include "Nave.h"
#include "Tiro.h"
#include "Defesa.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void criaInimigo(Inimigo foe)
{
    insere(inimigos, &foe, sizeof foe);
}

/*------------------------------------------------------------*/

void dispara(Inimigo *foe)
{
    Projetil bullet;

    bullet.x = foe->x;
    bullet.y = foe->y;
    bullet.z = foe->z;
    bullet.dano = BALA_DANO;

    miraProjetil(&bullet);
    aplicaPrecisao(&bullet, foe->precisao);
    criaProjetil(bullet);

    foe->espera = foe->cooldown;
}

/*------------------------------------------------------------*/

bool inimigoSaiu(Inimigo *foe)
{
    return (foe->z < nave.z);
}
