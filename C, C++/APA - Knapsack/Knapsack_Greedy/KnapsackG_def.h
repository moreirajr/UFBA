#ifndef KNAPSACKG_DEF_H_
#define KNAPSACKG_DEF_H_

int nItens;
double pesoMochila, *peso, *valor;

void peso_e_valor(char *s, double *Peso, double *Valor);
void ordenacao(double *valor, double *peso);
void Knapsack_Greedy();

#endif /* KNAPSACKG_DEF_H_ */

/*****************************************************************************/


