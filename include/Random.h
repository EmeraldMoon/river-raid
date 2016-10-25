/*------------------------------------------------------------------*
 |                                                                  |
 |  Funções que cuidam de geração (pseudo) aleatória de dados, por  |
 |  meio de distribuições de probabilidade.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*-------------------------*
 |   P R O T Ó T I P O S   |
 *-------------------------*----------------------------------------*/

/*
 *  Recebe dois valores reais tais que a <= b.
 *  Devolve um número real uniformemente no intervalo [a,b).
 */
double uniforme(double a, double b);

/*
 *  Gera um número real sob uma distribuição Normal com 
 *  média e desvio padrão estipulados.
 */
double normal(double media, double desvpadr);
