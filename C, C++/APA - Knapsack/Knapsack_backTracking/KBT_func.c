#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Kbt_def.h"


/*******************************************************************************/
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

/*******************************************************************************/

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

/*******************************************************************************/

void knapsack_backTrack(int i, double valorAtual, double pesoAtual){
	int j;

	if(pesoAtual + peso[i] <= pesoMochila){
		temp[i] = 1;
		if(i <= nItens)
			knapsack_backTrack(i+1, valorAtual + valor[i], pesoAtual + peso[i]);
		if((valorAtual + valor[i] > valorMax) && (i==nItens)){
			valorMax = valorAtual + valor[i];
			pesoMax = pesoAtual + peso[i];
			for(j=0; j<=i; j++)
				solucao[j] = temp[j];
		}
	}

	if(itemPromissor(i, valorAtual, pesoAtual) >= valorMax){
		temp[i] = 0;
		if(i<=nItens)
			knapsack_backTrack(i+1, valorAtual, pesoAtual);
		if((valorAtual > valorMax) && (i==nItens)){
			valorMax = valorAtual;
			pesoMax = pesoAtual;
			for(j=0; j<=i; j++)
				solucao[j] = temp[j];
		}
	}
}

/*******************************************************************************/

double itemPromissor(int i, double valorAtual, double pesoAtual){
	int j;
	double temp_valor = valorAtual;
	double temp_peso = pesoAtual;

	for(j=i; j<=nItens; j++){
		temp_peso += peso[j];
		if(temp_peso < pesoMochila)
			temp_valor += valor[j];
		else
			return (temp_valor + (1 -(temp_peso - pesoMochila) / (double)peso[j]) * valor[j]);

	}

	return temp_valor;
}
