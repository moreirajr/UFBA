#ifndef KBT_DEF_H_
#define KBT_DEF_H_

int nItens, *solucao, *temp;
double pesoMochila, valorMax, pesoMax, *valor, *peso;

void peso_e_valor(char *s, double *Peso, double *Valor);

void ordenacao(double *valor, double *peso);

void knapsack_backTrack(int i, double valorAtual, double pesoAtual);

double itemPromissor(int i, double valorAtual, double pesoAtual);

#endif /* KBT_DEF_H_ */
