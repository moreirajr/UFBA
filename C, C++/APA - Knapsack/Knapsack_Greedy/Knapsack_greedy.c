#include <stdio.h>
#include <stdlib.h>
#include "KnapsackG_def.h"

int main(){
	int i;
	char *pv_obj = NULL;
	peso = NULL;
	valor = NULL;

	printf("\nInforme o numero de itens\n");
	scanf("%d", &nItens);

	peso = (double*)calloc(nItens, sizeof(double));
	valor = (double*)calloc(nItens, sizeof(double));


	printf("\nInforme o peso da mochila\n");
	scanf("%lf", &pesoMochila);

	printf("\nValor, Peso.\n");
	for(i=0; i<nItens; i++){
			fflush(stdin);
			pv_obj = (char*)calloc(20, sizeof(char));
			fgets(pv_obj, 20, stdin);
			peso_e_valor(pv_obj, &peso[i], &valor[i]);
			fflush(stdin);			memset(pv_obj, 0, 20);
		}

	ordenacao(valor, peso);
	Knapsack_Greedy();

	getchar();

	return 0;
}
