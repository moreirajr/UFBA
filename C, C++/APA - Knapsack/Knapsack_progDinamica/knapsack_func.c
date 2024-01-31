#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "knapsack_def.h"



double max(double a, double b){
	if(a>b)
		return a;
	return b;
}

double **alocar_matriz(int i, int j){
	double **matriz;
	int k;

	if((i>0) && (j>0)){
		matriz = (double**)calloc(i, sizeof(double*));	//alocar linhas
		if(matriz==NULL){
			printf("\nErro ao alocar linhas da matriz.");
			return NULL;
		}

		for(k=0; k<j; k++){
			matriz[k] = (double*)calloc(j, sizeof(double));	//alocar colunas
			if(matriz[k]==NULL){
				printf("\nErro ao alocar colunas da matriz.");
				return NULL;
			}
		}
	}
	return matriz;
}

/*********************************************************************/

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

/*********************************************************************/

void knapsack_bin(int n, int m, double *p, double *v, double **knapsack){
	int i, j, aux, *temp=NULL;


	for(i=0; i<=n; i++)
		for(j=0; j<=m; j++){
			if((i==0) || (j==0))
				knapsack[i][j] = 0;
			else
			{
				if(j >= p[i]){
					aux = delta(p[i]);
					knapsack[i][j] = max (knapsack[i-1][j - aux] + v[i], knapsack[i-1][j]);
				}
				else
					knapsack[i][j] = knapsack[i-1][j];
			}
		}

	

	temp = (int*)calloc(n+1, sizeof(int));
	for(i=0; i<=n; i++)
		temp[i] = 0;


	for(i=n, j=m; (i>0)&&(j>0); i--)
		if(knapsack[i][j]!=knapsack[i-1][j]){
			temp[i] = 1;
			aux = delta(p[i]);
			j = j - aux;
		}

	//pesos para solucao otima (indices do vetor)
	for(i=1; i<=n; i++){
		if(temp[i]==1)
			printf("\nIndices do Peso:%d ", i);
	}

}

/*********************************************************************/

int delta(float d){
	int i;

	return (i = (int)round(d)); //ceil ou floor

}

/*********************************************************************/
