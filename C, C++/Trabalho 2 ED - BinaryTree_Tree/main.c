/* Compilador: MinGW (gcc)
 *	Aluno: Evaldo Machado Moreira Junior
 */

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include <string.h>

int main(void){
	Btree *dicionario=NULL;
	char opcao=0, palavra[31], p[31];
	int i=0, aux;

	while(opcao!='e'){
		opcao = getchar();
		switch(opcao){

		case 'p':	// * e o caractere que simboliza o final da palavra
			fflush(stdin);
			memset(palavra, 0, 31);
			gets(palavra);
			i = strlen(palavra);
			palavra[i] = '*';	palavra[i+1]=0;
			dicionario = inserir (dicionario, NULL, palavra, 0); // zero = indice do vetor
			i=0;		//passando NULL, pois nao existe no anterior
			break;

		case 't':
			//sugestao por troca, troca apenas 1 letra
			fflush(stdin);
			memset(palavra, 0, 31);
			memset(p, 0, 31);
			gets(palavra);
			i = strlen(palavra);
			palavra[i]='*';	palavra[i+1]=0;
			listar_palavras(dicionario, p, palavra, 0, 1); // zero = indice do vetor
			break;	//um='tipo' ,diferenciar entre listar todas palavras, e listar sugestoes por troca

		case 'i':
			//sugestao por insercao, insere no final da palavra
			fflush(stdin);
			memset(palavra, 0, 31);
			memset(p, 0, 31);
			gets(palavra);
			i = strlen(palavra);
			palavra[i]='*';	palavra[i+1]=0;	//p = temp, armazena a palavra imprimida
			sugestao_iR(dicionario, palavra, p, 0, 0); //i=0, j=0 (indices do vetor)
			break;

		case 'r':
			//sugestao por remocao, remove apenas a ultima letra
			fflush(stdin);
			memset(palavra, 0, 31);
			memset(p, 0, 31);
			gets(palavra);
			i = strlen(palavra);
			palavra[i-2]='*';	palavra[i-1]=0; //removendo um caractere da palavra
			sugestao_iR(dicionario, palavra, p, 0, 0);
			break;

		case 'o':
			fflush(stdin);
			memset(palavra, 0, 31);
			listar_palavras(dicionario, palavra, palavra, 0, 2); //0 = indice do vetor usado
			break;	//2 = 'tipo', diferenciar entre listar e sugestao por troca

		case 'a':
			fflush(stdin);
			imprimir_arvore(dicionario);
			break;

		case 'n':
			fflush(stdin);
			scanf("%d", &i);
			aux = 0;	//contador
			imprimir_nivel(dicionario, i, 1, &aux);	//admitindo que o nivel começa do 1
			break;	//1 = aux, incrementado ate a condicao i=aux (aux, parametro da funcao)

		case 'd':
			fflush(stdin);
			gets(palavra);
			i = strlen(palavra);
			palavra[i]='*';	palavra[i+1]=0;
			dicionario =  remover_palavra(dicionario, NULL, palavra, 0);
			memset(palavra, 0, 31);
			break;

		case 'e':
			deletar_arvore(dicionario);	//deletar estrutura
			break;

		default:
			fflush(stdin);
			break;
		}
	}
	return 0;
}
