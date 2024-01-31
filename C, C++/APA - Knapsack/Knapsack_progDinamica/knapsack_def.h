#ifndef KNAPSACK_F_H_
#define KNAPSACK_F_H_


double max(double a, double b);
int delta(float d);
double **alocar_matriz(int i, int j);
void peso_e_valor(char *s, double *Peso, double *Valor);
void knapsack_bin(int n, int m, double *p, double *v, double **knapsack);


#endif /* KNAPSACK_F_H_ */
