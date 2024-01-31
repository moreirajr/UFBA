#include <stdio.h>
#include <stdlib.h>
#include "KnapsackG_def.h"
/***************************************************************************/

void peso_e_valor(char *s, double *Peso, double *Valor){
	int i=0, aux;
	char *temp_peso = (char*)calloc(strlen(s), sizeof(char))
		, *temp_valor = (char*)calloc(strlen(s), sizeof(char));


	while(s[i]!=','){
		temp_valor[i] = s[i];
		i++;
	}

	temp_valor[i]='\0';
	(*Valor) = atof(temp_valor);
	i+=2;
	aux = i;

	while(s[i]!='\0'){
		temp_peso[i-aux] = s[i];
		i++;
	}

	temp_peso[i]='\0';
	(*Peso) = atof(temp_peso);

}

/***************************************************************************/

void ordenacao(double *valor, double *peso){
	int i,j;
	double *val, aux_1;

	val = (double*)calloc(nItens, sizeof(double));

	for(i=0; i<nItens; i++)
		val[i] = (valor[i] / peso[i]);

	for(i=0; i<nItens-1; i++)
		for(j=i+1; j<nItens; j++)
			if(val[i] < val[j]){
				aux_1 = val[i];		//mudar val (valor / peso)
				val[i] = val[j];
				val[j] = aux_1;
				aux_1 = peso[i];	//mudar peso
				peso[i] = peso[j];
				peso[j] = aux_1;
				aux_1 = valor[i];	//mudar valor
				valor[i] = valor[j];
				valor[j] = aux_1;
			}
}

/***************************************************************************/

void Knapsack_Greedy(){
	double valorTotal = 0.0, pesoTemp = pesoMochila;
	int i, j, *solucao;


	solucao = (int*)calloc(nItens, sizeof(int));

	for(i=0; i<nItens; i++)
		solucao[i] = 0;

	for(i=0; i<nItens; i++)
		if(peso[i] <= pesoTemp){
			solucao[i] = 1;
			valorTotal += valor[i];
			pesoTemp -= peso[i];
		}
		else
			break;

	valorTotal += (pesoTemp/peso[i]) * valor[i];

	printf("\nItens escolhidos: ");
	for(i=0; i<nItens; i++)
		if(solucao[i]==1)
			printf("%d, ", i);

	printf("\nValor max: %lf", valorTotal);



}
