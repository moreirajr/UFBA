#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"

//Evaldo M. Moreira Junior
//arquivo utilizado: registroHash.bin



int main(int argc, char *argv[]){
	char comando, nome[20];
	unsigned int chave, idade;

	/****************** criando o arquivo caso nao exista, ou carregando ******************/
	if ((arquivo_hashing = fopen(ARQUIVO, "rb"))== NULL){
		inicializar();	//tabela hash
		criar_arquivo();
	}
	else{
		fread(&apontador_R, sizeof(unsigned int), 1, arquivo_hashing);
		fclose(arquivo_hashing);
	}



	metodo = getchar();


	/****************** menu ******************/
	while(comando!='e'){
		fflush(stdin);
		//memset(chave_aux, 0, 20);
		//memset(idade_aux, 0, 3);
		comando = getchar();

		switch(comando){

		case 'i':
			fflush(stdin);
			//gets(chave_aux);
			scanf("%d\n", &chave);
			gets(nome);
			//gets(idade_aux);
			scanf("\n%d", &idade);	//scanf("\n%d", &idade);

			if(consultar(chave, 0)==1)
				inserir_chave(nome, idade, chave);
			break;

		case 'c':
			fflush(stdin);
			scanf("%d", &chave);
			consultar(chave, 1);
		break;

		case 'r':
			fflush(stdin);
			scanf("%d", &chave);
			consultar(chave, 2);
			break;

		case 'p':
			imprimir();
		break;

		case 'm':
			media_acessos();
			break;

		case 'e':	//salvar_arquivo();
		break;	//exit

		default:
		break;

		}
	}

	return 0;
}


