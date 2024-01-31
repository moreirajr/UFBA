#include "def.h"


void inicializar(){
	int i, j;

	pagina_hash = (hash_linear*) malloc((N*sizeof(hash_linear)));
	// pagina_hash = (hash_linear*) calloc(N, sizeof(hash_linear));


	for(i=0; i<N; i++){
		for(j=0; j<REGISTROS_POR_PAGINA; j++){
			pagina_hash[i].pagina.chave[j] = -1;
			pagina_hash[i].pagina.idade[j] = -1;
			strcpy(pagina_hash[i].pagina.nome[j].rNome, "\0");
			pagina_hash[i].pagina.overflow = NULL;
		}

		pagina_hash[i].pagina.numRegInseridos = 0;

	}

	hashinfo_nln.level = 0;
	hashinfo_nln.next = 0;
	hashinfo_nln.prev = 0;
	hashinfo_nln.num_paginas = N;
	hashinfo_nln.num_paginas_overflow = 0;
	hashinfo_nln.numRegInseridosOverflow = 0;
	hashinfo_nln.numRegInseridos = 0;


}

/********************************************************************/

void carregar_arquivo(){

	int i, j;

	pagina_hash = (hash_linear*) malloc((hashinfo_nln.num_paginas*sizeof(hash_linear)));

	if((arquivo_hash = fopen(ARQUIVO1, "rb"))== NULL){
		printf("\nErro ao abrir arquivo %s!", ARQUIVO1);
		return;
	}

	rewind(arquivo_hash);


	for(i=0; i < hashinfo_nln.num_paginas; i++){
		fseek(arquivo_hash, sizeof( int)*(i+1), SEEK_SET);
		for(j=0; j < REGISTROS_POR_PAGINA; j++){
			fread(&pagina_hash[i].pagina.chave[j], sizeof(int), 1, arquivo_hash);
			fread(&pagina_hash[i].pagina.idade[j], sizeof(int), 1, arquivo_hash);
			fread(&pagina_hash[i].pagina.nome[j].rNome, sizeof(char)*21, 1, arquivo_hash);

		}
		fread(&pagina_hash[i].pagina.numRegInseridos, sizeof(int), 1, arquivo_hash);
	}







	fclose(arquivo_hash);




	/*if ((arquivo_info = fopen(ARQUIVO2, "r"))== NULL){
		printf("\nErro ao abrir arquivo %s!", ARQUIVO2);
		return;
	}
	if(fread(&hashinfo_nln, sizeof(struct hashinfo), 1, arquivo_info) !=1)
		printf("\nErro leitura do arquivo %s!", ARQUIVO2);
	fclose(arquivo_info);
	*/
}

/********************************************************************/

void salvar_arquivo(){
	int i, j;

	if ((arquivo_hash = fopen(ARQUIVO1, "wb"))== NULL){
			printf("\nErro ao abrir arquivo %s!", ARQUIVO1);
			return;
		}

//	for(i=0; i < hashinfo_nln.num_paginas; i++){
//		fseek(arquivo_hash, sizeof(struct page)*(i+1), SEEK_SET);
//		fwrite(&pagina_hash[i].pagina, sizeof(struct page), 1, arquivo_hash);
//	}


	for(i=0; i < hashinfo_nln.num_paginas; i++){
			fseek(arquivo_hash, sizeof(int)*(i+1), SEEK_SET);
			for(j=0; j < REGISTROS_POR_PAGINA; j++){
				fwrite(&pagina_hash[i].pagina.chave[j], sizeof(int), 1, arquivo_hash);
				fwrite(&pagina_hash[i].pagina.idade[j], sizeof(int), 1, arquivo_hash);
				fwrite(&pagina_hash[i].pagina.nome[j].rNome, sizeof(char)*21, 1, arquivo_hash);

			}
			fread(&pagina_hash[i].pagina.numRegInseridos, sizeof(int), 1, arquivo_hash);
		}





	fclose(arquivo_hash);



	/*if ((arquivo_info = fopen(ARQUIVO2, "w"))== NULL){
		printf("\nErro ao abrir arquivo %s!", ARQUIVO2);
		return;
		}
	if(fwrite(&hashinfo_nln, sizeof(struct hashinfo), 1, arquivo_info) !=1)
		printf("\nErro na escrita do arquivo %s!", ARQUIVO2);
	fclose(arquivo_info);
	*/
}


/********************************************************************/

int h(int chave, int level){
	int aux = (int) pow(2.0, level);

	return (chave % (N*aux));
}

/********************************************************************/

void inicializar_pagina(int indice, int tipo){
	int i;
	paginaDeRegistro *pagina_aux;

	if(tipo==0){	//pagina primaria
		for(i=0; i < REGISTROS_POR_PAGINA; i++){
			pagina_hash[indice].pagina.chave[i] = -1;
			pagina_hash[indice].pagina.idade[i] = -1;
			strcpy(pagina_hash[indice].pagina.nome[i].rNome, "\0");
		}

		pagina_hash[indice].pagina.overflow = NULL;
		pagina_hash[indice].pagina.numRegInseridos = 0;
		hashinfo_nln.num_paginas++;
	}
	else{	//pagina overflow

		if(pagina_hash[indice].pagina.overflow==NULL){
			if( (pagina_hash[indice].pagina.overflow = (paginaDeRegistro*) malloc(sizeof(struct page))) ==NULL)
				printf("\nErro ao alocar pagina overflow");

			for(i=0; i< REGISTROS_POR_PAGINA; i++){
				pagina_hash[indice].pagina.overflow->chave[i] = -1;
				pagina_hash[indice].pagina.overflow->idade[i] = -1;
				strcpy(pagina_hash[indice].pagina.overflow->nome[i].rNome, "\0");
			}

			pagina_hash[indice].pagina.overflow->overflow = NULL;
			pagina_hash[indice].pagina.overflow->numRegInseridos = 0;

		} //se a pagina for null
		else
		{
			pagina_aux = pagina_hash[indice].pagina.overflow;

			while(pagina_aux->overflow!=NULL)
				pagina_aux = pagina_aux->overflow;

			if((pagina_aux->overflow = (paginaDeRegistro*) malloc(sizeof(struct page))) == NULL)
				printf("\nErro ao alocar pagina overflow");

			for(i=0; i<REGISTROS_POR_PAGINA; i++){
				pagina_aux->overflow->chave[i] = -1;
				pagina_aux->overflow->idade[i] = -1;
				strcpy(pagina_aux->overflow->nome[i].rNome, "\0");
			}
			pagina_aux->overflow->overflow = NULL;
			pagina_aux->overflow->numRegInseridos = 0;
		}

		hashinfo_nln.num_paginas_overflow++;

	}
}
/********************************************************************/

void inserir(int chave){
	int  i, m;
	paginaDeRegistro *aux;

	m= h(chave, hashinfo_nln.level);


	if(m < hashinfo_nln.next)
		m= h(chave, hashinfo_nln.level+1);

	if(pagina_hash[m].pagina.numRegInseridos < REGISTROS_POR_PAGINA){
		for(i=0; i<REGISTROS_POR_PAGINA; i++)
			if(pagina_hash[m].pagina.chave[i] == -1){
				pagina_hash[m].pagina.chave[i] = chave;
				pagina_hash[m].pagina.idade[i] = idade;
				strcpy(pagina_hash[m].pagina.nome[i].rNome, nome);
				pagina_hash[m].pagina.numRegInseridos++;
				break;
		}

		hashinfo_nln.numRegInseridos++;
	}
	else	//pagina overflow sera criada
	{

		if(pagina_hash[m].pagina.overflow==NULL)
			inicializar_pagina(m, 1);

		if(pagina_hash[m].pagina.overflow->numRegInseridos < REGISTROS_POR_PAGINA){
			for(i=0; i<REGISTROS_POR_PAGINA; i++)
				if(pagina_hash[m].pagina.overflow->chave[i] == -1){
					pagina_hash[m].pagina.overflow->chave[i] = chave;
					pagina_hash[m].pagina.overflow->idade[i] = idade;
					strcpy(pagina_hash[m].pagina.overflow->nome[i].rNome, nome);
					pagina_hash[m].pagina.overflow->numRegInseridos++;
					break;
				}
		}	//pagina overflow cheia
		else
		{ //overflow na pagina overflow

			aux = pagina_hash[m].pagina.overflow;

			while((aux->overflow!=NULL) && (aux->numRegInseridos==REGISTROS_POR_PAGINA))
				aux = aux->overflow;

			if(aux->numRegInseridos < REGISTROS_POR_PAGINA){
				for(i=0; i < REGISTROS_POR_PAGINA; i++)
					if(aux->chave[i]==-1){
						aux->chave[i] = chave;
						aux->idade[i] = idade;
						strcpy(aux->nome[i].rNome, nome);
						aux->numRegInseridos++;
						break;
					}
			}
			else
			{
				inicializar_pagina(m,1);	//criar outra pagina overflow
				aux->overflow->chave[0] = chave;
				aux->overflow->idade[0] = idade;
				strcpy(aux->overflow->nome[0].rNome, nome);
				aux->overflow->numRegInseridos++;
			}
		}

		hashinfo_nln.numRegInseridosOverflow++;
	}

	while( ( armazenamento() ) > UTILIZACAO_MAX)
		split();

}

/********************************************************************/

void imprimir(){
	int i, j;
	paginaDeRegistro *aux_print;

	printf("nivel: %d", hashinfo_nln.level);
	printf("\nnext: %d", hashinfo_nln.next);

	for(i=0; i<hashinfo_nln.num_paginas; i++){
		printf("\npagina: %d", i);
		for(j=0; j<REGISTROS_POR_PAGINA; j++)
			if(pagina_hash[i].pagina.chave[j]!=-1)
				printf("\nchave: %d nome: %s idade: %d", pagina_hash[i].pagina.chave[j],
				pagina_hash[i].pagina.nome[j].rNome, pagina_hash[i].pagina.idade[j]);

		if(pagina_hash[i].pagina.overflow!=NULL){
			aux_print = pagina_hash[i].pagina.overflow;

			while(aux_print!=NULL){
				for(j=0; j < REGISTROS_POR_PAGINA; j++){
					if(aux_print->chave[j]!=-1){
						printf("\n----------");
						printf("\nchave %d nome: %s idade: %d", aux_print->chave[j], aux_print->nome[j].rNome, aux_print->idade[j]);
					}
				}
				aux_print = aux_print->overflow;
			}//end while
		}//pagina overflow!=NULL
	}//for
}

/********************************************************************/

void split(){
	int aux_chave, aux_idade, i, j, aux, count = 0;
	char aux_nome[21];
	int m;
	paginaDeRegistro *pagina_aux;




	pagina_hash = (hash_linear*) realloc(pagina_hash, (hashinfo_nln.num_paginas +1)*sizeof(hash_linear));
	inicializar_pagina(hashinfo_nln.num_paginas, 0);


	for(i=0; i<REGISTROS_POR_PAGINA; i++){
		if(h(pagina_hash[hashinfo_nln.next].pagina.chave[i], hashinfo_nln.level+1) != hashinfo_nln.next){
			aux_chave = pagina_hash[hashinfo_nln.next].pagina.chave[i];	//remover da pagina e add na nova pagina
			aux_idade = pagina_hash[hashinfo_nln.next].pagina.idade[i];
			strcpy(aux_nome, pagina_hash[hashinfo_nln.next].pagina.nome[i].rNome);

			m = h(pagina_hash[hashinfo_nln.next].pagina.chave[i], hashinfo_nln.level+1);
			remover(hashinfo_nln.next, i, 0, count); //indice da pagina, indice do registro
			reinserir(m, aux_chave, aux_idade, aux_nome);
		}
	}// pagina primaria

	if(pagina_hash[hashinfo_nln.next].pagina.overflow!=NULL){//split na pagina overflow
		pagina_aux = pagina_hash[hashinfo_nln.next].pagina.overflow;

		while(pagina_aux!=NULL){

		for(i=0; i<REGISTROS_POR_PAGINA; i++)
			if(h(pagina_aux->chave[i], hashinfo_nln.level+1) != hashinfo_nln.next){
				aux_chave = pagina_aux->chave[i];
				aux_idade =pagina_aux->idade[i];
				strcpy(aux_nome, pagina_aux->nome[i].rNome);

				m = h(pagina_aux->chave[i], hashinfo_nln.level+1);
				remover(hashinfo_nln.next, i, 1, count);	//add um count
				reinserir(m, aux_chave, aux_idade, aux_nome);
			}
			else{

				if(pagina_hash[hashinfo_nln.next].pagina.numRegInseridos < REGISTROS_POR_PAGINA)
					for(j=0; j < REGISTROS_POR_PAGINA; j++)
						if(pagina_hash[hashinfo_nln.next].pagina.chave[j] == -1){
							aux_chave = pagina_aux->chave[i];
							aux_idade =pagina_aux->idade[i];
							strcpy(aux_nome, pagina_aux->nome[i].rNome);

							m = h(pagina_aux->chave[i], hashinfo_nln.level+1);

							remover(hashinfo_nln.next, i, 1, count);
							reinserir(m, aux_chave, aux_idade, aux_nome);

							if(pagina_aux==NULL)
								break;
						}
			}

		pagina_aux = pagina_aux->overflow;
		count++;	//identificar qual pagina overflow sera utilizada

		}
	}

	aux = (int) pow(2.0, hashinfo_nln.level);

	if(hashinfo_nln.next + 1 >= N * aux){
		hashinfo_nln.prev = hashinfo_nln.next;
		hashinfo_nln.next = 0;
		hashinfo_nln.level++;
	}
	else{
		hashinfo_nln.prev = hashinfo_nln.next;
		hashinfo_nln.next++;
		}

}

/********************************************************************/

void reinserir(int m, int rchave, int ridade, char rnome[21]){
	int regInserido = 0, i; // m = h(rchave, hashinfo_nln.level+1);
	paginaDeRegistro *pagina_aux;


	if(pagina_hash[m].pagina.numRegInseridos < REGISTROS_POR_PAGINA){
		for(i=0; i<REGISTROS_POR_PAGINA; i++)
			if(pagina_hash[m].pagina.chave[i] == -1){
				pagina_hash[m].pagina.chave[i] = rchave;
				strcpy(pagina_hash[m].pagina.nome[i].rNome, rnome);
				pagina_hash[m].pagina.idade[i] = ridade;
				pagina_hash[m].pagina.numRegInseridos++;
				hashinfo_nln.numRegInseridos++;
				break;
			}
	}
	else{//overflow

		if(pagina_hash[m].pagina.overflow==NULL)
			inicializar_pagina(m,1);

			pagina_aux = pagina_hash[m].pagina.overflow;

			while(pagina_aux!=NULL){
				if(pagina_aux->numRegInseridos < REGISTROS_POR_PAGINA)
					for(i=0; i < REGISTROS_POR_PAGINA; i++)
						if(pagina_aux->chave[i]==-1){
							regInserido = 1;
							pagina_aux->chave[i] = rchave;
							strcpy(pagina_aux->nome[i].rNome, rnome);
							pagina_aux->idade[i] = ridade;
							pagina_aux->numRegInseridos++;
							break;
						}

				if(pagina_aux->overflow==NULL)
					break;
				pagina_aux = pagina_aux->overflow;

			}

			if(regInserido==0){
				if(pagina_aux->overflow==NULL)
					inicializar_pagina(m,1);
				pagina_aux->chave[0] = rchave;
				strcpy(pagina_aux->nome[0].rNome, rnome);
				pagina_aux->idade[0] = ridade;
				pagina_aux->numRegInseridos++;
			}
			hashinfo_nln.numRegInseridosOverflow++;
		}
}


/********************************************************************/

float armazenamento(){
	float storage_util = 0, storage_max = (hashinfo_nln.num_paginas + hashinfo_nln.num_paginas_overflow) * REGISTROS_POR_PAGINA;

	storage_util = hashinfo_nln.numRegInseridos + hashinfo_nln.numRegInseridosOverflow;

	return ((storage_util / storage_max) *100);
}

/********************************************************************/

void remover(int indice, int k, int tipo, int count){
	paginaDeRegistro *pagina_aux;


	if(tipo==0){
		pagina_hash[indice].pagina.chave[k] = -1;
		pagina_hash[indice].pagina.idade[k] = -1;
		strcpy(pagina_hash[indice].pagina.nome[k].rNome, "\0");
		pagina_hash[indice].pagina.numRegInseridos--;
		hashinfo_nln.numRegInseridos--;
	}
	else{

		pagina_aux = pagina_hash[indice].pagina.overflow;

		while(count>0){
			pagina_aux = pagina_aux->overflow;
			count--;
		}

		pagina_aux->chave[k] = -1;
		pagina_aux->idade[k] = -1;
		strcpy(pagina_aux->nome[k].rNome, "\0");
		pagina_aux->numRegInseridos--;

		if(pagina_aux->numRegInseridos==0){
			if(pagina_aux->overflow!=NULL)
				pagina_aux = pagina_aux->overflow;
			else
				pagina_aux = NULL;

			hashinfo_nln.num_paginas_overflow--;
		}
		hashinfo_nln.numRegInseridosOverflow--;
	}
}

/********************************************************************/

int consultar(int chave, int func_call){	//0 = consulta, 1 = insercao, 2 media de acesso
	paginaDeRegistro *pagina_aux;
	int i, count =0, m = h(chave, hashinfo_nln.level);


	if(hashinfo_nln.numRegInseridos==0){
		if(func_call==0)
			printf("chave nao encontrada: %d", chave);
		else
			return 1;
	}
	else
	{

	if(m < hashinfo_nln.next)
		m = h(chave, hashinfo_nln.level+1);

	count++;

	for(i=0; i < REGISTROS_POR_PAGINA; i++)
		if(pagina_hash[m].pagina.chave[i] == chave){
			if(func_call==1){
				printf("chave ja existente: %d", chave);
				return 0;
			}
			if(func_call==0){
				printf("chave: %d\n%s\n%d", pagina_hash[m].pagina.chave[i], pagina_hash[m].pagina.nome[i].rNome, pagina_hash[m].pagina.idade[i]);
				return 0;
			}
			if(func_call==2)
				return count;
		}

	if(pagina_hash[m].pagina.overflow!=NULL){
		pagina_aux = pagina_hash[m].pagina.overflow;

		while(pagina_aux!=NULL){
			count++;
			for(i=0; i < REGISTROS_POR_PAGINA; i++)
				if(pagina_aux->chave[i] == chave){
					if(func_call==1){
						printf("chave ja existente: %d", chave);
						return 0;
					}
					if(func_call==0){
						printf("chave: %d\n%s\n%d", pagina_aux->chave[i], pagina_aux->nome[i].rNome, pagina_aux->idade[i]);
						return 0;
					}
					if(func_call==2)
						return count;
			}

		pagina_aux = pagina_aux->overflow;

		}
	}
		if(func_call==0)
			printf("chave nao encontrada: %d", chave);

	}

	return 1;
}

/********************************************************************/

void remover_registro(int chave){
	int i, j, m, m2, count = 0, aux_chave, aux_idade, reg_remov = 0;
	char aux_nome[21];
	paginaDeRegistro *pagina_aux;

	m= h(chave, hashinfo_nln.level);
		if(m < hashinfo_nln.next)
			m= h(chave, hashinfo_nln.level+1);

		for(i=0; i < REGISTROS_POR_PAGINA; i++)
			if(pagina_hash[m].pagina.chave[i]==chave){
				reg_remov = 1;
				remover(m, i, 0, count);
				if(pagina_hash[m].pagina.overflow!=NULL){ //subir os registros pra pagina principal
					pagina_aux = pagina_hash[m].pagina.overflow;
					while(pagina_aux->overflow!=NULL){
						count++;
						pagina_aux = pagina_aux->overflow;
					}
					for(j=REGISTROS_POR_PAGINA; j >=0; j--)
						if(pagina_aux->chave[j]!=-1){
							aux_chave = pagina_aux->chave[j];
							aux_idade = pagina_aux->idade[j];
							strcpy(aux_nome, pagina_aux->nome[j].rNome);

							m2 = h(pagina_aux->chave[j], hashinfo_nln.level+1);
							remover(m, j, 1, count);
							reinserir(m2, aux_chave, aux_idade, aux_nome);
						}
				}//overflow!=NULL
			break;
			}

		if(reg_remov==0)//procurar na pagina overflow
			if(pagina_hash[m].pagina.overflow!=NULL){
				pagina_aux = pagina_hash[m].pagina.overflow;

				while(pagina_aux->overflow!=NULL){
					count++;
					pagina_aux = pagina_aux->overflow;
				}

				for(i=0; i < REGISTROS_POR_PAGINA; i++)
					if(pagina_aux->chave[i]==chave){
						reg_remov = 1;
						remover(m, i, 1, count);
						break;
					}

			}



	if(reg_remov==1){
		if(hashinfo_nln.num_paginas>N)	//nao diminuir alem do numero inicial de paginas
			while(( armazenamento() ) < UTILIZACAO_MIN)
				balancear();
	}
	else
		printf("chave nao encontrada: %d", chave);

}

/********************************************************************/

void media_de_acessos(){
	int i, j;
	float total = 0;
	paginaDeRegistro *pagina_aux;


	for(i=0; i < hashinfo_nln.num_paginas; i++){
		for(j=0; j < REGISTROS_POR_PAGINA; j++)
			if(pagina_hash[i].pagina.chave[j]!=-1)
				total += consultar(pagina_hash[i].pagina.chave[j], 2);

		if(pagina_hash[i].pagina.overflow!=NULL){
			pagina_aux = pagina_hash[i].pagina.overflow;

			while(pagina_aux!=NULL){
				for(j=0; j < REGISTROS_POR_PAGINA; j++)
					if(pagina_aux->chave[j]!=-1)
						total += consultar(pagina_aux->chave[j], 2);
				pagina_aux = pagina_aux->overflow;
			}
		}//paginas overflow!=null
	}

	printf("%.1f", total/(hashinfo_nln.numRegInseridos+hashinfo_nln.numRegInseridosOverflow));
}

/********************************************************************/

void balancear(){
	int i, m, count = 0, aux, aux_idade, aux_chave;
	char aux_nome[21];

	paginaDeRegistro *pagina_aux;

	for(i=0; i <pagina_hash[hashinfo_nln.num_paginas-1].pagina.numRegInseridos ; i++)
		if(pagina_hash[hashinfo_nln.num_paginas-1].pagina.chave[i]!=-1){

		m = h(pagina_hash[hashinfo_nln.num_paginas-1].pagina.chave[i], hashinfo_nln.level-1);

		aux_chave = pagina_hash[hashinfo_nln.num_paginas-1].pagina.chave[i];
		aux_idade = pagina_hash[hashinfo_nln.num_paginas-1].pagina.idade[i];
		strcpy(aux_nome, pagina_hash[hashinfo_nln.num_paginas-1].pagina.nome[i].rNome);

		remover(hashinfo_nln.num_paginas-1, i, 0, count);

		reinserir(m, aux_chave, aux_idade, aux_nome

				);


	}

	if(pagina_hash[hashinfo_nln.num_paginas-1].pagina.overflow!=NULL){
		pagina_aux = pagina_hash[hashinfo_nln.num_paginas-1].pagina.overflow;

		while(pagina_aux!=NULL){
			for(i=0; i < REGISTROS_POR_PAGINA; i++){
				m = h(pagina_aux->chave[i], hashinfo_nln.level-1);

				reinserir(m, pagina_aux->chave[i], pagina_aux->idade[i], pagina_aux->nome[i].rNome);
				remover(hashinfo_nln.num_paginas-1, i, 0, count);
			}

			count++;
			pagina_aux = pagina_aux->overflow;
		}

	}


	if(hashinfo_nln.next==0){
		if(hashinfo_nln.prev!=0){	//Next + 1 >= N * (2^level) no split
			aux = hashinfo_nln.next;
			hashinfo_nln.next = hashinfo_nln.prev;
			hashinfo_nln.prev = aux - 1;
		}
	}
	else{
		aux = hashinfo_nln.next;
		hashinfo_nln.next = hashinfo_nln.prev;
		if(hashinfo_nln.prev==0){
			hashinfo_nln.prev = (N-1) + (hashinfo_nln.level-1);
		}
		else
			hashinfo_nln.prev = hashinfo_nln.next-1;
	}

	free(&pagina_hash[hashinfo_nln.num_paginas-1]);


		pagina_hash = (hash_linear*) realloc(pagina_hash, (hashinfo_nln.num_paginas -1)*sizeof(hash_linear));
		hashinfo_nln.num_paginas--;



}

/********************************************************************/
