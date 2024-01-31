/*
 *	Aluno: Evaldo Machado Moreira Junior
 */

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include <string.h>

/*******************************************************************************************/
Btree *inserir(Btree *arv, Btree *arv_anterior, char palavra[31], int i){


	if(arv==NULL){
		arv = (Btree*)malloc(sizeof(Btree));
		arv->dir = NULL;
		arv->esq = NULL;
		arv->letra = palavra[i];
		if(palavra[i+1]!=0)	//continuar inserindo, ate palavra[i]='*', (0=NULL, ASCII)
			arv->esq = inserir(arv->esq, arv, palavra, i+1);
	}

	else{	//procurar a direita, caso nao encontre, criara um novo filho com a letra
		if(arv->letra < palavra[i])
			arv->dir = inserir(arv->dir, arv, palavra, i);
		else
			if(arv->letra > palavra[i]){
				Btree *temp=NULL;
				if(arv_anterior!=NULL){
					if(arv_anterior->esq==arv){
						temp = inserir_letra (temp, arv, palavra[i]);
						arv = temp;
						arv_anterior->esq = arv;
						arv->esq = inserir(arv->esq, arv, palavra, i+1);
					}
					else
						if(arv_anterior->dir==arv){
							temp = inserir_letra(temp, arv, palavra[i]);
							arv = temp;
							arv_anterior->dir = arv;
							arv->esq = inserir(arv->esq, arv, palavra, i+1);
						}
					}
				else{	//arv_anterior=NULL
					temp = inserir_letra(temp, arv, palavra[i]);
					arv = temp;
					arv->esq = inserir(arv->esq, arv, palavra, i+1);
					}
				}
			else	//letra encontrada, passando para a proxima letra
				if(arv->letra==palavra[i]){
					if((arv->esq->letra=='*') && (palavra[i+1]=='*'))
						printf("palavra ja existente");
					else
						arv->esq = inserir(arv->esq, arv, palavra, i+1);
				}
			}
	return arv;
}

/*******************************************************************************************/
void listar_palavras(Btree *arv, char palavra[31], char temp[31], int i, int OP){


	if(arv==NULL)
		return;

	if(arv->letra=='*'){
		if(OP==1){	//sugestao por troca
			int a, n=0;
			for(a=0; a<strlen(palavra); a++)
				if(palavra[a]==temp[a])
					n++;
		if(n==( strlen(temp) -2) )
			puts(palavra);
		}
		else
			puts(palavra);
		if(arv->dir!=NULL)
			palavra[i]=0;
		else
			palavra[i-1]=0;
	}
	else{
		palavra[i]= arv->letra;
		palavra[i+1] = 0;
	}

	listar_palavras(arv->esq, palavra, temp, i+1, OP);
	listar_palavras(arv->dir, palavra, temp, i, OP);

}

/*******************************************************************************************/
void imprimir_nivel(Btree *arv, int n, int aux, int *count){

	if(arv==NULL)
		return;

	if(n==1)
		printf("%c", arv->letra);
	else{
		if(n==aux){
			if( (*count)>=1)
				printf(", %c", arv->letra);
			else
				printf("%c", arv->letra);
			(*count)++;
		}

		imprimir_nivel(arv->esq, n, aux+1, count);
		imprimir_nivel(arv->dir, n, aux+1, count);

	}
}

/*******************************************************************************************/
void imprimir_arvore(Btree *arv){

	if(arv==NULL)
		return;
//imprimir em ordem

	imprimir_arvore(arv->esq);

	if((arv->esq!=NULL) && (arv->dir!=NULL) )
		printf("chave: %c filho esquerdo: %c filho direito: %c\n", arv->letra, arv->esq->letra, arv->dir->letra);
	else{
		if((arv->esq==NULL) && (arv->dir==NULL)){
			printf("chave: %c filho esquerdo: nil filho direito: nil\n", arv->letra);
		}
		else{
			if(arv->esq==NULL)
				printf("chave: %c filho esquerdo: nil filho direito: %c\n", arv->letra, arv->dir->letra);
			else
				printf("chave: %c filho esquerdo: %c filho direito: nil\n", arv->letra, arv->esq->letra);

		}
	}
	imprimir_arvore(arv->dir);

}

/*******************************************************************************************/

void deletar_arvore(Btree *arv){

	if(arv==NULL)
		return;

	deletar_arvore(arv->esq);
	deletar_arvore(arv->dir);
	free(arv);
}

/*******************************************************************************************/

Btree *remover_palavra(Btree *arv, Btree *arv_ant, char palavra[31], int i){

	if(arv==NULL){
		printf("palavra inexistente");
		return NULL;
	}

	if(arv->letra!=palavra[i]){
		if(arv->dir!=NULL){
			arv->dir = remover_palavra(arv->dir, arv, palavra, i);
		}
		else
			if((i<=strlen(palavra)-1) && (arv->letra!=palavra[i]))
				printf("palavra inexistente");
	}
	else
		if(arv->letra==palavra[i])
			if(arv->esq!=NULL)
				arv->esq = remover_palavra(arv->esq, arv, palavra, i+1);

	//a remocao comeca sempre da ultima letra da palavra
	if((arv->dir==NULL) && (arv->esq==NULL) && (arv->letra==palavra[i])){
		if(arv_ant!=NULL){
			if(arv_ant->dir==arv)	//fazendo o anterior apontar para a arvore (evitando loop infinito)
				arv_ant->dir=NULL;
			else
				if(arv_ant->esq==arv)
					arv_ant->esq=NULL;
		} //se arv_anterior=NULL, nesse caso, entao so ha um no na arvore
		free(arv);
		arv=NULL;
	}
	else{	//a palavra a ser removida eh parte de outra palavra
		if((arv->dir) && (arv->esq==NULL) && (arv->letra==palavra[i])){
			Btree *temp=arv;
			if(arv_ant!=NULL){
				arv = arv->dir;
				if(arv_ant->dir==temp)
					arv_ant->dir = arv;
				else
					if(arv_ant->esq==temp)
						arv_ant->esq = arv;
			}
			else //arv_ant =NULL
				arv = arv->dir;

			free(temp);
			temp=NULL;
		}
	}
	return arv;
}

/*******************************************************************************************/

void sugestao_iR(Btree *arv, char palavra[31], char temp[31], int i, int j){
	int n;

	if(arv==NULL)
		return;


	if(arv->letra!=palavra[i]){
			if(palavra[i+1]!=0)
				sugestao_iR(arv->dir, palavra, temp, i, j);
		}
		else
			if(arv->letra==palavra[i])
				if(palavra[i+1]!=0){
					temp[i] = palavra[i];	//palavra a ser imprimida
					temp[i+1] = 0;
					sugestao_iR(arv->esq, palavra, temp, i+1, j+1);
				}

	if(palavra[i]=='*'){
		if(arv->letra=='*'){
			n = strlen(temp);
			temp[n]='*';	temp[n+1]=0;	//apenas para a comparacao
			if(strcmp(palavra, temp)!=0){
				temp[n]=0;
				if(n==strlen(palavra))
				puts(temp);
				if(arv->dir!=NULL){
					temp[j]=0;
				}
				else
					temp[j-1]=0;
			}
			temp[n]=0;
		}
		else{ //arv->letra!= '*'
			temp[j]=arv->letra;	//copiando para o vetor temporaria, as sugestoes
			temp[j+1]=0;
		}
	
		sugestao_iR(arv->esq, palavra, temp, i, j+1);
		sugestao_iR(arv->dir, palavra, temp, i, j);


	}
}

/*******************************************************************************************/

Btree *inserir_letra(Btree *arv, Btree *arv_prox, char letra){

	/*funcao usada para inserir um no entre dois outros nos, para inserir em ordem na arvore*/
	if(arv==NULL){
		arv = (Btree*)malloc(sizeof(Btree));
		arv->letra = letra;
		arv->esq=NULL;
		arv->dir = arv_prox;
	}

	return arv;

}

/*******************************************************************************************/
