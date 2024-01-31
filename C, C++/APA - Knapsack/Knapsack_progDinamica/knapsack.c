#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "knapsack_def.h"

/*	Evaldo M. Moreira Junior
 *
 * Problema da mochila binario usando programacao dinamica, O(Nm)
 */


int main (){
	double **bag=NULL, *peso=NULL, *valor=NULL, b;
	int n, i=0, m;
	char *pv_obj = NULL;

	printf("\nNumero de objetos: ");
	scanf("%d", &n);

	peso = (double*)calloc(n, sizeof(double));
	valor = (double*)calloc(n, sizeof(double));

	printf("\nPeso max da mochila: "); //peso
	scanf("%lf", &b);

	printf("\nFator de aproximacao delta: ");	//delta, aproximacao
	//scanf("%lf", &d);
	m = delta(b);


	bag = alocar_matriz(n,m);


	printf("\nValor, Peso.\n");
	for(i=1; i<=n; i++){
		fflush(stdin);
		pv_obj = (char*)calloc(20, sizeof(char));
		fgets(pv_obj, 20, stdin);
		peso_e_valor(pv_obj, &peso[i], &valor[i]);
		fflush(stdin);			memset(pv_obj, 0, 20);
	}


	knapsack_bin(n, m, peso, valor, bag);


	getchar();

	free(bag);
	free(peso);
	free(valor);
	free(pv_obj);
	return 0;
}
