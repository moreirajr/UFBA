#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"

//Aluno: Evaldo M. Moreira Junior
//arquivo utilizado: registroArvoreB.bin


int main(int argc, char *argv[]){
	char comando, nome[20];
	unsigned int chave, idade;
	int aux;



	/*criando o arquivo caso nao exista, ou carregando */
		if ((arquivo_arvoreB = fopen(ARQUIVO, "rb"))== NULL){
			inicializar(&no);
			criar_arquivo();
		}
		else
		{
			fread(&prox_no, sizeof(int), 1, arquivo_arvoreB);

			fseek(arquivo_arvoreB, sizeof(int), SEEK_SET);
			fread(&num_nodes, sizeof(int), 1, arquivo_arvoreB);

			fseek(arquivo_arvoreB, 2*sizeof(int), SEEK_SET);
			fread(&num_regs_armazenados, sizeof(int), 1, arquivo_arvoreB);

			fclose(arquivo_arvoreB);
		}


	arquivo_arvoreB = fopen(ARQUIVO, "rb+");

		/* menu */
	while(comando!='e'){
		fflush(stdin);
		comando = getchar();
		switch(comando){

			case 'i':
			fflush(stdin);
			scanf("%d\n", &chave);
			gets(nome);
			scanf("\n%d", &idade);
			inserir(nome, idade, chave, 1, 0, 0);
			ultimo_no_inserido = 0;
			break;

			case 'c':
			fflush(stdin);
			scanf("%d", &chave);
			aux = 0;
			consultar(chave, 1, 'c', &aux);
			break;

			case 'r':
			fflush(stdin);
			scanf("%d", &chave);
			remover(chave);
			break;

			case 'p':
			imprimir(1);
			break;

			case 'o':
			print_ordem_crescente(1);
			break;

			case 'e':
			fclose(arquivo_arvoreB);
			break;

			default:
			break;

		}
	}

	return 0;
}
