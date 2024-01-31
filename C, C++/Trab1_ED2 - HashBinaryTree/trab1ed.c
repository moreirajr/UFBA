#include "def.h"



int main(int argc, char *argv[]){
	char comando;
	unsigned int chave, temp=0;
	controle = &temp;



	/****************** criando o arquivo caso nao exista, ou carregando ******************/
	if ((reg_hash = fopen(ARQUIVO, "rb"))== NULL){
		inicializar();	//tabela hash
		salvar_arquivo();
	}
	else{
		carregar_arquivo();
	}


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
			//chave = atoi(chave_aux);, chave aux e idade strings (erro
			//idade = atoi(idade_aux);
			if(consultar(chave, 0)==1)
				inserir(nome, chave, idade, 0);
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


		case 't':
			fflush(stdin);
			scanf("%d", &chave);
			inserir('\0', chave, -1, 1);
			break;


		case 'm':
			media_acessos();
			break;


		case 'e':	salvar_arquivo();
			break;	//exit


		default:
			break;
		}
	}

	return 0;
}


