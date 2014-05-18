#include "Cenario.h"
#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Random.h"

/* Elementos básicos do jogo */
Nave nave;
Lista *inimigos;
Lista *projeteis;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void inicializa()
{
    criaNave(0, VIDAS_INI);
    inimigos  = criaLista();
    projeteis = criaLista();
}

/*------------------------------------------------------------*/

void atualizaCenario()
{
    Celula *p;

    moveNave();

    /* Em ambos os laços, não há necessidade de avançar 'p'
       em caso de remoção, pois a função exclui() já o faz. */
    p = inimigos;
    while (p->prox != NULL) {
        Inimigo *foe = p->prox->item;
        if ((foe->base.espera)-- == 0) inimigoDispara(foe);
        if (inimigoSaiu(foe)) exclui(p);
        else p = p->prox;
    }
    p = projeteis;
    while (p->prox != NULL) {
        Projetil *bullet = p->prox->item;
        moveProjetil(bullet);
        if (projetilAcertou(bullet) == NAVE) {
            nave.base.hp -= bullet->dano;
            exclui(p);
        }
        else if (projetilSaiu(bullet)) exclui(p);
        else p = p->prox;
    }

    /* Verifica se nave perdeu vida */
    if (nave.base.hp <= 0) {
        (nave.vidas)--;
        if (nave.vidas >= 0) criaNave(nave.base.z, nave.vidas);
    }
}

/*------------------------------------------------------------*
 *  
 *  Esta função é um modelo de testes. No futuro, a 
 *  geração de inimigos deve ser feita de forma pré-determinada.
 *
 */
void geraInimigo()
{
    Inimigo foe;

    /* (uniforme(-1, 0) | 1) gera um número que é 1 ou -1 */
    foe.base.x = uniforme(X_MAX/2, X_MAX) * (uniforme(-1, 0) | 1);
    foe.base.y = uniforme(0, Y_MAX);
    foe.base.z = nave.base.z + Z_MAX;

    foe.base.hp       = FOE_HP;
    foe.base.cooldown = uniforme(10, 20);
    foe.base.espera   = foe.base.cooldown;
    foe.base.raio     = FOE_RAIO;
    foe.base.altura   = FOE_ALTURA;
    foe.precisao      = uniformeD(0.5, 1.0);

    criaInimigo(foe);
}

/*------------------------------------------------------------*/

void liberaCenario()
{
    liberaLista(inimigos);
    liberaLista(projeteis);
}
