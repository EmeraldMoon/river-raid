#include <cstdlib>  /* rand */
#include <cmath>    /* sqrt, log */

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

double uniforme(double a, double b)
{
    double x = (double) rand()/RAND_MAX;
    return (x * (b - a)) + a;
}

/*------------------------------------------------------------------*
 *
 *  O código a seguir foi inspirado na biblioteca StdRandom 
 *  (em java) do livro "Algorithms", de Sedgewick & Wayne.
 *
 */
double normal(double media, double desvpadr)
{
    double r, x, y;
    do {
        x = uniforme(-1.0, 1.0);
        y = uniforme(-1.0, 1.0);
        r = x*x + y*y;
    } while (r >= 1 || r == 0);
    
    /* Valor gerado por uma Normal(0, 1) */
    r = x * sqrt((-2) * log(r)/r);

    /* Aplicamos média e desvio-padrão */
    return media + (desvpadr * r);
}