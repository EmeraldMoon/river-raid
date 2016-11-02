#include <cstdlib>  /* free */

#include "lista.hpp"
#include "base.hpp"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

Lista *criaLista()
{
    Celula *ini = (    Celula *) mallocSafe(sizeof *ini);
    ini->prox = NULL;
    return ini;
}

/*------------------------------------------------------------------*/

void listaInsere(Celula *ini, void *item)
{
    Celula *nova = (    Celula *) mallocSafe(sizeof *nova);
    nova->item = item;
    nova->prox = ini->prox;
    ini->prox = nova;   
}

void listaRemoveProx(Celula *cel)
{
    Celula *morta = cel->prox;
    cel->prox = morta->prox;
    free(morta->item);
    free(morta);
}

/*------------------------------------------------------------------*/

void liberaLista(Celula *ini)
{
    Celula *cel = ini->prox;
    while (cel != NULL) {
        Celula *morta = cel;
        cel = cel->prox;
        free(morta->item);
        free(morta);
    }
    free(ini);
}
