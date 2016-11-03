#include <random>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Objeto do gerador de nºs aleatórios */
std::default_random_engine generator;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

double uniforme(double a, double b)
{   
    std::uniform_real_distribution<> distribution(a, b);
    return distribution(generator);
}

double normal(double media, double desvpadr)
{
    std::normal_distribution<> distribution(media, desvpadr);
    return distribution(generator);
}
