#include <string.h>  /* memcpy */
#include "Lista.h"
#include "Base.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

Lista * criaLista()
{
    Celula *ini = mallocSafe(sizeof (Celula));
    ini->prox = NULL;
    return ini;
}

/*------------------------------------------------------------------*
 *
 *  É necessário que a função receba o tamanho em bytes a ser
 *  alocado e copiado, pois sizeof não funciona com (void *).
 *
 */
void insere(Celula *ini, void *item, size_t nbytes)
{
    Celula *nova = mallocSafe(sizeof (Celula));
    nova->item = mallocSafe(nbytes);
    memcpy(nova->item, item, nbytes);
    nova->prox = ini->prox;
    ini->prox = nova;   
}

/*------------------------------------------------------------------*/

void exclui(Celula *cel)
{
    Celula *morta = cel->prox;
    cel->prox = morta->prox;
    free(morta->item); morta->item = NULL;
    free(morta); morta = NULL;
}

/*------------------------------------------------------------------*
 *
 *  Por questões de segurança (e paranoia), todos os ponteiros
 *  pós-liberação são apontados para NULL.
 *
 */
void liberaLista(Celula *ini)
{
    Celula *cel = ini->prox;
    while (cel != NULL) {
        Celula *aux = cel->prox;
        free(cel->item); cel->item = NULL;
        free(cel); cel = NULL;
        cel = aux;
    }
    free(ini); ini = NULL;
}
