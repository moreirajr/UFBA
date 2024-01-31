#include "def.h"


int main(int argc, char *argv[]){
	char comando;
	unsigned int chave;



	if((arquivo_hash = fopen(ARQUIVO1, "rb"))== NULL){
		inicializar();
		fclose(arquivo_hash);
		salvar_arquivo();

	}
	else{
		carregar_arquivo();
	}


	while(comando!='e'){
		fflush(stdin);
		comando = getchar();

		switch(comando){

		case 'i':	fflush(stdin);
					scanf("%d\n", &chave);
					gets(nome);
					scanf("\n%d", &idade);	//scanf("\n%d", &idade);
					if(consultar(chave, 1)==1)
						inserir(chave);
					break;

		case 'c':	fflush(stdin);
					scanf("%d", &chave);
					consultar(chave, 0);
					break;

		case 'r':	fflush(stdin);
					scanf("%d", &chave);
					remover_registro(chave);
					break;

		case 'p':	imprimir();
					break;


		case 'm':	media_de_acessos();
					break;

		case 'e':	salvar_arquivo();

					break;

		default: break;

		}
	}

	return 0;
}
