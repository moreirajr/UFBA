#include "definicoes.h"

//arquivo utilizado: registroHash.bin

void inicializar(){
	//int i;

	/*for(i=0; i<TAMANHO_ARQUIVO; i++){
		tabela_hash[i].chave = -1;
		strcpy(tabela_hash[i].nome, "\0");
		tabela_hash[i].idade = -1;
		tabela_hash[i].cadeia = -1;
	}
	*/

	tabela_h.chave = -1;
	strcpy(tabela_h.nome, "\0");
	tabela_h.idade = -1;
	tabela_h.cadeia = -1;

	apontador_R = TAMANHO_ARQUIVO - 1;
}

/*******************************************************/


void carregar_arquivo(){//nao utilizada
	int i;

	if((arquivo_hashing = fopen(ARQUIVO, "rb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	rewind(arquivo_hashing);
	fread(&apontador_R, sizeof(unsigned int), 1, arquivo_hashing);

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		fseek(arquivo_hashing, ((i+1)*sizeof(struct REG)) + sizeof(unsigned int), SEEK_SET);
		if(fread(&tabela_hash[i], sizeof(struct REG), 1, arquivo_hashing) !=1){
			printf("\nErro ao ler arquivo!");
			if(feof(arquivo_hashing))
				break;
		}
	}

	fclose(arquivo_hashing);
}

/*******************************************************/

void criar_arquivo(){
	int i;

	if ((arquivo_hashing = fopen(ARQUIVO, "wb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	rewind (arquivo_hashing);
	fwrite(&apontador_R, sizeof(unsigned int), 1, arquivo_hashing);

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		fseek(arquivo_hashing, ((i+1)*sizeof(struct REG)) + sizeof(unsigned int), SEEK_SET);
		if(fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing) !=1){
			printf("\nErro na escrita do arquivo!");
			break;
		}
	}

	fclose(arquivo_hashing);
}

/*******************************************************/

unsigned int h(unsigned int chave){

	return (chave % TAMANHO_ARQUIVO);
}

/*******************************************************/

void ajustar_apontador(int R){
	int aux;

	if(R>apontador_R)
		apontador_R = R;

	aux = apontador_R;

	if(apontador_R>0)
		do{
			if(apontador_R==-1)
				break;

			fseek(arquivo_hashing, ((apontador_R+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

			if(tabela_h.chave==-1)
				break;

			apontador_R--;

		}while(tabela_h.chave!=-1);

	if(apontador_R!=-1){
		rewind(arquivo_hashing);
		fwrite(&apontador_R, sizeof(unsigned int), 1, arquivo_hashing);
	}
	else
		apontador_R = aux;
}


/*******************************************************/

void inserir_chave(char nome[20], int idade, int chave){
	int aux, indice = h(chave), ajuste=0;
	registro tabela_aux;


	tabela_aux.chave = chave;
	strcpy(tabela_aux.nome, nome);
	tabela_aux.idade = idade;
	tabela_aux.cadeia = -1;

	if((arquivo_hashing = fopen(ARQUIVO, "rb+"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	rewind(arquivo_hashing);

	fseek(arquivo_hashing, ((indice+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
	fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);


	if(tabela_h.chave==-1){	//vazio
		fseek(arquivo_hashing, ((indice+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fwrite(&tabela_aux, sizeof(struct REG), 1, arquivo_hashing);
		if(indice==apontador_R)
			ajuste=1;
	}

	else

	{//colisao
		ajuste = 1;

		if(tabela_h.cadeia==-1){//sem cadeia
			tabela_h.cadeia = apontador_R;
			fseek(arquivo_hashing, ((indice+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

			fseek(arquivo_hashing, ((apontador_R + 1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fwrite(&tabela_aux, sizeof(struct REG), 1, arquivo_hashing);
		}

		else

		{//cadeia com 2 ou mais elementos

			if(metodo=='e'){//EISCH
				aux = tabela_h.cadeia;
				tabela_h.cadeia = apontador_R;

				fseek(arquivo_hashing, ((indice+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
				fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

				tabela_aux.cadeia = aux;

				fseek(arquivo_hashing, ((apontador_R + 1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
				fwrite(&tabela_aux, sizeof(struct REG), 1, arquivo_hashing);
			}
			else{//LISCH
				aux = tabela_h.cadeia;

				while(tabela_h.cadeia!=-1){
					aux = tabela_h.cadeia;
					fseek(arquivo_hashing, ((aux+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
					fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

				}

				tabela_h.cadeia = apontador_R;
				fseek(arquivo_hashing, ((aux+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
				fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);


				fseek(arquivo_hashing, ((apontador_R + 1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
				fwrite(&tabela_aux, sizeof(struct REG), 1, arquivo_hashing);

			}

		}//cadeia com 2 ou mais elementos

	//apontador_R = ajustar_apontador(apontador_R);
	//rewind(arquivo_hashing);
	//fwrite(&apontador_R,sizeof(unsigned int), 1, arquivo_hashing);

	}//colisao

	if(ajuste==1)
		ajustar_apontador(apontador_R);

	fclose(arquivo_hashing);
}

/*******************************************************/


void imprimir(){
	int i;

	if((arquivo_hashing = fopen(ARQUIVO, "rb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
		}

	fread(&apontador_R, sizeof(unsigned int), 1, arquivo_hashing);


	fseek(arquivo_hashing, (sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);

	//printf("\nApontador R: %d\n", apontador_R);

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		//fseek(arquivo_hashing, ((i+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);
		if(tabela_h.chave==-1){
			printf("%d: vazio\n", i);
		}
		else{
			printf("%d: %d %s %d", i, tabela_h.chave, tabela_h.nome, tabela_h.idade);
		if(tabela_h.cadeia==-1)
			printf(" nulo\n");
		else
			printf(" %d\n", tabela_h.cadeia);
		}
	}

	fclose(arquivo_hashing);

}

/*******************************************************/

int consultar(unsigned int chave, int OP){
	int indice = h(chave), count_Acessos=0, indice_aux, indice_anterior;
	registro *tabela_aux=NULL;

	indice_aux = indice;
	indice_anterior = indice;

	if(OP!=3)
		if((arquivo_hashing = fopen(ARQUIVO, "rb"))== NULL){
			printf("\nErro ao abrir arquivo!");
			exit(EXIT_FAILURE);
		}

	//le a posicao do indice na tabela, no arquivo
	fseek(arquivo_hashing, ((indice+1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
	fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

	if(tabela_h.chave==-1){
		if((OP==1) || (OP==2))
			printf("chave nao encontrada: %d",chave);
	}//posicao vazia

	else
	{//posicao ocupada

		do{
			count_Acessos++;


			tabela_aux = (registro*)realloc(tabela_aux, count_Acessos * sizeof(struct REG));
			tabela_aux[count_Acessos-1] = tabela_h;

			if(tabela_h.chave==chave){
				if(OP!=3)
					fclose(arquivo_hashing);
				if(OP==1)
					printf("chave: %d\n%s\n%d", chave, tabela_h.nome, tabela_h.idade);
				else
					if(OP==0)
						printf("chave ja existente: %d", chave);
				if(OP==2)
					remover(indice_aux, indice_anterior);	//indice = inicio da cadeia

				if(OP==3)
					return count_Acessos;
				return 0;
			}

			if(tabela_h.cadeia==-1)
				break;

			indice_anterior = indice_aux;
			indice_aux = tabela_h.cadeia;

			fseek(arquivo_hashing, ((tabela_h.cadeia + 1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

		}while((tabela_h.cadeia!=-1) || (tabela_h.chave!=-1) );

		if(tabela_h.cadeia==-1){
			if((OP==1) || (OP==2))
				printf("chave nao encontrada: %d", chave);
		}
	}//posicao ocupada

	if(OP!=3)
		fclose(arquivo_hashing);

	return 1;
}

/*******************************************************/

void remocao(unsigned int indice_atual, unsigned int indice_anterior){
	registro vazio;
	int apontador_aux, homeAddressElem=0,
	indice_aux = indice_atual,
	indice_aux_anterior = indice_anterior;

	vazio.chave = -1;
	strcpy(vazio.nome, "\0");
	vazio.idade = -1;
	vazio.cadeia = -1;

	fseek(arquivo_hashing, ((indice_atual+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
	fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

	if(tabela_h.cadeia==-1){

		apontador_aux = tabela_h.cadeia;
		fseek(arquivo_hashing, ((indice_anterior+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);
		tabela_h.cadeia = apontador_aux;
		fseek(arquivo_hashing, ((indice_anterior+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

		fseek(arquivo_hashing, ((indice_atual+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fwrite(&vazio, sizeof(struct REG), 1, arquivo_hashing);

	}
	else
	{	//apontador do indice a ser removido
		apontador_aux = tabela_h.cadeia;

		do{

			indice_aux_anterior = indice_aux;	//indice_atual
			indice_aux = tabela_h.cadeia;

			fseek(arquivo_hashing, ((tabela_h.cadeia + 1) * sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

			if(h(tabela_h.chave)==indice_atual){//procurar elemento com home address na posicao do elemento a ser removido
				homeAddressElem = 1;
				tabela_h.cadeia = apontador_aux;
				fseek(arquivo_hashing, ((indice_atual+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
				fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

				remocao(indice_aux, indice_aux_anterior);

				break;
			}

		}while(tabela_h.cadeia!=-1);



		if(homeAddressElem==0){	//nenhum elemento movido
			fseek(arquivo_hashing, ((indice_anterior+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

			tabela_h.cadeia = apontador_aux;
			fseek(arquivo_hashing, ((indice_anterior+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fwrite(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);

			fseek(arquivo_hashing, ((indice_atual+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
			fwrite(&vazio, sizeof(struct REG), 1, arquivo_hashing);

		}
	}


	ajustar_apontador(indice_atual);

}

/*******************************************************/

void remover(unsigned int indice, unsigned int indice_anterior){


	arquivo_hashing = fopen(ARQUIVO, "rb+");

	remocao(indice, indice_anterior);

	fclose(arquivo_hashing);
}

/*******************************************************/

void media_acessos(){
	int i, count = 0, aux, num_reg = 0;
	float resultado, total = 0;

	if((arquivo_hashing = fopen(ARQUIVO, "rb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	fseek(arquivo_hashing,  sizeof(struct REG) + sizeof(unsigned int), SEEK_SET);

	for(i=0; i < TAMANHO_ARQUIVO; i++){
		//fseek(arquivo_hashing, ((i+1)* sizeof(struct REG) ) + sizeof(unsigned int), SEEK_SET);
		fread(&tabela_h, sizeof(struct REG), 1, arquivo_hashing);
		if(tabela_h.chave != -1){
			num_reg++;
			aux = consultar(tabela_h.chave, 3);
			total++;	//total de registro na tabela
			count = count + aux;	//numero de acessos
		}
	}

	if(num_reg>0){
		resultado = (count/total);
		printf("%.1f", resultado);
	}

	fclose(arquivo_hashing);

}

/*******************************************************/
