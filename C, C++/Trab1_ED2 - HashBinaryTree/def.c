#include "def.h"



void inicializar(){
	int i;

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		tabela_hash[i].chave = -1;
		strcpy(tabela_hash[i].nome, "\0");
		tabela_hash[i].idade = -1;
	}

}

/********************************************************************/

void carregar_arquivo(){
	int i;

	if((reg_hash = fopen(ARQUIVO, "rb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		if(fread(&tabela_hash[i], sizeof(struct REG), 1, reg_hash) !=1){
			printf("\nErro ao ler arquivo!");
			if(feof(reg_hash))
				break;
		}
	}
	fclose(reg_hash);
}

/********************************************************************/

void salvar_arquivo(){
	int i;

	if ((reg_hash = fopen(ARQUIVO, "wb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	for(i=0; i<TAMANHO_ARQUIVO; i++){
		if(fwrite(&tabela_hash[i], sizeof(struct REG), 1, reg_hash) !=1){
			printf("\nErro na escrita do arquivo!");
			break;
		}
	}

	fclose(reg_hash);
}

/********************************************************************/
unsigned int h1(unsigned int chave){
	//retorna o resto da divisao
	return (chave % TAMANHO_ARQUIVO);
}
/********************************************************************/
unsigned int h2(unsigned int chave){
	//retorna o quociente da divisao
	return (chave / TAMANHO_ARQUIVO);
}
/********************************************************************/

void inserir(char nome[20], unsigned int chave, unsigned int idade, int OP){
	unsigned int m, m2;
	int i=1;

	arvore *pai=NULL, *arv_hash=NULL;

	m = h1(chave);

	if((tabela_hash[m].chave==-1) ||(tabela_hash[m].chave==-2)){	//insere no h1 da chave
		if(OP==0){
		strcpy(tabela_hash[m].nome, nome);
		tabela_hash[m].chave = chave;
		tabela_hash[m].idade = idade;
		}
		else
			tentativa_insercao(m);
	}
	else{

		m2 = h2(chave);

		//for(i=1; i<TAMANHO_ARQUIVO; i++)
		while( (*controle==0) && (i < TAMANHO_ARQUIVO)){	// 0 = procurando chave, 1 = inserido
			arv_hash = criar_arvore(arv_hash, pai, chave, m2 , m, i, 0, OP);
			i++;
		}

		*controle = 0;
	}
}

/********************************************************************/

arvore* criar_arvore(arvore *tree, arvore *pai, unsigned int chave, unsigned int incremento, int indice,  int level, int mover, int OP){
//OP: 0 insercao, 1 tentativa de insercao

	if(*controle==0){
		if(level==1){
			if((tabela_hash[indice].chave==-1) ||(tabela_hash[indice].chave==-2)){	//indice da tabela hash esta vazio
				if(tree==NULL){
					tree = criar_no(tree, pai, indice, chave);
					if(OP==0){	//inserir a chave no registro
						tabela_hash[indice].chave = chave;
						strcpy(tabela_hash[indice].nome, nome);
						tabela_hash[indice].idade = idade;
					}
					else
						tentativa_insercao(indice);

					*controle=1;
					return tree;
				}
			}
			else{	//indice da tabela hash ocupado
				if(tree==NULL){
					tree = criar_no(tree, pai, indice,tabela_hash[indice].chave);	//passando a chave que ocupa o indice na tabela
					if(OP==1)
						tentativa_insercao(indice);
				}
			}
		}
		else
			if(level>1){
				tree->filho_esq = criar_arvore(tree->filho_esq, tree, chave, incremento, h1(tree->indice+incremento), level-1, mover, OP);
				tree->filho_dir = criar_arvore(tree->filho_dir, tree, chave, h2(tree->chave), h1(tree->indice+h2(tree->chave)), level-1, mover+1, OP);
//a chave e sempre inserida no primeiro no vazio, e entao e realizada a movimentacao, caso seja necessario
				if((*controle==1)){
					if(tree->filho_dir==NULL)	//o no a ser movido esta na esquerda
						mover_no(tree->filho_esq, mover);
					else	//o no a ser movido esta na direita
						mover_no(tree->filho_dir, mover+1);
					*controle=2;
					}
			}
	}

	return tree;
}

/********************************************************************/

arvore *criar_no(arvore *node, arvore *pai, unsigned int indice, unsigned int chave){

	node = (arvore*)malloc(sizeof(arvore));
	node->pai = (arvore*)malloc(sizeof(arvore));
	node->pai = pai;
	node->filho_esq = NULL;
	node->filho_dir = NULL;
	node->indice = indice;
	node->chave = chave;

	return node;
}

/********************************************************************/

void mover_no(arvore* tree, int mover){	//mover: numero de movimentos
	arvore *aux=tree;
	int indice_aux;

	indice_aux = tree->indice;	//o valor inserido, que mudara de posicao


	while(mover>0){
		if(aux->pai!=NULL){
			if(aux->pai->filho_esq!=NULL){
				if(aux->pai->filho_esq->chave== aux->chave){	//nao movimenta
					aux = aux->pai;
				}
				else{	//movimenta

				trocar_valores(indice_aux, aux->pai->indice);
				indice_aux = aux->pai->indice;
				aux = aux->pai;
				mover--;
				}
			}
		}
		else{//aux->pai==NULL
			if(aux->filho_dir->indice == indice_aux ){
				trocar_valores(aux->indice, indice_aux);
				mover--;
			}
		}
	}//end while
}

/********************************************************************/

void trocar_valores(int a, int b){
	int chave_aux, idade_aux;
	char nome_aux[20];

	chave_aux = tabela_hash[a].chave;
	strcpy(nome_aux, tabela_hash[a].nome);
	idade_aux = tabela_hash[a].idade;

	tabela_hash[a].chave = tabela_hash[b].chave;
	strcpy(tabela_hash[a].nome, tabela_hash[b].nome);
	tabela_hash[a].idade = tabela_hash[b].idade;

	tabela_hash[b].chave = chave_aux;
	strcpy(tabela_hash[b].nome, nome_aux);
	tabela_hash[b].idade = idade_aux;
}

/********************************************************************/
int consultar(unsigned int chave, int OP){
	//OP: 0 insercao, 1 consulta, 2 remover, 3 media de acessos
	int m = h1(chave), m2, aux = tabela_hash[m].chave, end_loop=0, count=0;


	if(tabela_hash[m].chave == -1){
		if((OP==1) || (OP==2))
			printf("chave nao encontrada: %d",chave);
	}
	else{

		count ++;

		if(tabela_hash[m].chave==chave){
			if(OP==1)
				printf("chave: %d\n%s\n%d", chave, tabela_hash[m].nome, tabela_hash[m].idade);
			else
				if(OP==0)
					printf("chave ja existente: %d", chave);

			if(OP==2)
				remover(m);
			if(OP==3)
				return count;

			return 0;
		}

		count++;
		m2 = h2(chave);
		m = h1(m + m2);

		while(end_loop==0){
			if((tabela_hash[m].chave==-1) || (tabela_hash[m].chave==aux) ){	//achou uma posicao vazia ou a chave inicial
				if((OP==1) || (OP==2))
					printf("chave nao encontrada: %d", chave);
				end_loop=1;
				count = 0;
			}
			else
				if(tabela_hash[m].chave==chave){
					if(OP==1)
						printf("chave: %d\n%s\n%d", chave, tabela_hash[m].nome, tabela_hash[m].idade);
					else
						if(OP==0)
							printf("chave ja existente: %d", chave);
					end_loop=1;

					if(OP==2)
						remover(m);
					if(OP==3)
						return count;

					return 0;
				}

				count++;
				m = h1(m + m2);
		}
	}

	if(OP==3)
		return count;

	return 1;
}


/********************************************************************/

void imprimir(){
	int i;

	//escrever vazio se for o caso, * se o reg foi apagado
	for(i=0; i<TAMANHO_ARQUIVO; i++){
		if(tabela_hash[i].chave==-1){
			printf("%d: vazio\n", i);
		}
		else
			if(tabela_hash[i].chave==-2)
				printf("%d: *\n", i);
			else
				printf("%d: %d %s %d\n", i, tabela_hash[i].chave, tabela_hash[i].nome, tabela_hash[i].idade);
	}
}

/********************************************************************/

void remover(unsigned int indice){

	tabela_hash[indice].chave = -2;		//indice removido
	tabela_hash[indice].idade = -1;
	strcpy(tabela_hash[indice].nome, "\0");

}

/********************************************************************/

void tentativa_insercao(unsigned int indice){

	if(tabela_hash[indice].chave==-1)
		printf("%d: vazio", indice);
	else
		printf("%d: %d\n", indice, tabela_hash[indice].chave);
}

/********************************************************************/

void media_acessos(){
	int i, count = 0, aux;
	float resultado, total = 0;

	for(i=0; i < TAMANHO_ARQUIVO; i++)
		if((aux = consultar(tabela_hash[i].chave, 3))!=0){
			total++;	//total de registro na tabela
			count = count + aux;	//numero de acessos
		}

	resultado = (count/total);

	printf("%f", resultado);


}

/********************************************************************/
