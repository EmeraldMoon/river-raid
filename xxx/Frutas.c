#include <stdio.h>
#include "Cenario.h"

typedef struct{
    double x;
    double y;
} Limao;

typedef struct {
    int a;
    int b;
} Morango;

int main()
{
    int i;
    Limao lim;
    Morango mor;
    Lista *limao   = criaLista();
    Lista *morango = criaLista();
    Celula *p, *q;

    for (i = 0; i < 5; i++) {
        lim.x = (double) i/10;
        lim.y = (double) i/5;
        mor.a = i;
        mor.b = 10*i;
        insere(limao,   &lim, sizeof lim);
        insere(morango, &mor, sizeof mor);
    }
    for (p = limao, q = morango; p->prox != NULL && q->prox != NULL; p = p->prox, q = q->prox) {
        Limao   *l = p->prox->item;
        Morango *m = q->prox->item;
        printf("%3g %3g %3d %3d\n", l->x, l->y, m->a, m->b);
    }
    liberaLista(limao);
    liberaLista(morango);

    return 0;
}