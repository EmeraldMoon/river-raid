#include <stdio.h>  /* perror */
#include "Base.h"

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void * mallocSafe(size_t nbytes)
{
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        perror("ERRO");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
