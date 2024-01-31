#include <stdio.h>
#include <stdlib.h>
#include "Kbt_def.h"



int main(){
	int i;
	char *pv_obj = NULL;
	double  valorOtimo = 0.0;
	valorMax = -1;
	pesoMax = 0.0;

	printf("\nInforme o numero de itens\n");
	scanf("%d", &nItens);

	peso = (double*)calloc(nItens, sizeof(double));
	valor = (double*)calloc(nItens, sizeof(double));
	solucao = (int*)calloc(nItens, sizeof(int));
	temp = (int*)calloc(nItens, sizeof(int));

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
	for(i=0; i<nItens;i++)printf("valor: %lf\n",valor[i]);
	knapsack_backTrack(0, 0.0, 0.0);


	for(i=0; i<nItens; i++)
		if(solucao[i]==1){
			printf("\nValor: %lf, Item: %d", valor[i],i+1);
			valorOtimo += valor[i] * solucao[i];
		}
	printf("\nValor otimo: %lf", valorOtimo);

	getchar();



	return 0;
}
