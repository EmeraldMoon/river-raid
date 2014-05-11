#include "Nave.h"
#include "Defesa.h"
#include "Tiro.h"
#include "Cenario.h"
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
    criaNave();
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
        if ((foe->espera)-- == 0) dispara(foe);
        if (inimigoSaiu(foe)) exclui(p);
        else p = p->prox;
    }
    p = projeteis;
    while (p->prox != NULL) {
        Projetil *bullet = p->prox->item;
        moveProjetil(bullet);
        if (projetilAcertou(bullet)) {
            nave.hp -= bullet->dano;
            exclui(p);
        }
        else if (projetilSaiu(bullet)) exclui(p);
        else p = p->prox;
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
    foe.x = uniforme(X_MAX/2, X_MAX) * (uniforme(-1, 0) | 1);
    foe.y = uniforme(0, Y_MAX);
    foe.z = nave.z + Z_MAX;

    foe.precisao = uniformeD(0.5, 1.0);
    foe.hp       = FOE_HP;
    foe.cooldown = uniforme(10, 20);
    foe.espera  = foe.cooldown;

    criaInimigo(foe);
}

/*------------------------------------------------------------*/

void liberaCenario()
{
    liberaLista(inimigos);
    liberaLista(projeteis);
}
