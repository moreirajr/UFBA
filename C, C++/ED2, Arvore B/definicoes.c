#include "definicoes.h"
#include <math.h>

//Aluno: Evaldo M. Moreira Junior

/*################################################################################*/

void inicializar(node *temp){

	inicializar_no(temp);
	prox_no = 2;
	num_nodes = 1;
	num_regs_armazenados = 0;
}

/*################################################################################*/

void inicializar_filhos(int *filho_1, int *filho_2){
	int i;

	for(i=0; i < ORDEM +1; i++){
		filho_1[i] = 0;
		filho_2[i] = 0;
	}
}

/*################################################################################*/

 void inicializar_no(node *temp){
	int i;

	for(i=0; i < 2*ORDEM; i++){
		temp->registro[i] = criar_registro("\0", -1, -1);
		temp->filhos[i] = 0;
	}

	temp->filhos[2*ORDEM] = 0;

}

/*################################################################################*/

void criar_arquivo(){
	//escrever a raiz no arquivo

	if ((arquivo_arvoreB = fopen(ARQUIVO, "wb"))== NULL){
		printf("\nErro ao abrir arquivo!");
		return;
	}

	salvar_pos_vazia();
	salvar_num_nodes();
	salvar_num_regs();

	//a raiz comeca no indice 1(e e sempre armazenada no indice 1)
	salvar_No(&no, 1);

	fclose(arquivo_arvoreB);
}

/*################################################################################*/

void salvar_No(node *b_node, int pos_arquivo){

	fseek(arquivo_arvoreB, (pos_arquivo*sizeof(struct NO)) + (3*sizeof(int)), SEEK_SET);
	fwrite(b_node, sizeof(struct NO), 1, arquivo_arvoreB);

}

/*################################################################################*/

void ler_No(node *b_node, int pos_arquivo){
	fseek(arquivo_arvoreB, (pos_arquivo*sizeof(struct NO)) + (3*sizeof(int)), SEEK_SET);
	fread(b_node, sizeof(struct NO), 1, arquivo_arvoreB);
}

/*################################################################################*/

void salvar_pos_vazia(){
	rewind(arquivo_arvoreB);
	fwrite(&prox_no, sizeof(int), 1, arquivo_arvoreB);
}

/*################################################################################*/

void ajustar_prox_pos_vazia(){
	int i=1;
	node temp;


	while(i <= num_nodes){
		ler_No(&temp, i);
		if(temp.registro[0].chave==-1)
			break;
		i++;
	}

	if(i==num_nodes)
		prox_no = num_nodes+1;
	else
		prox_no = i;

	salvar_pos_vazia();

}

/*################################################################################*/

void salvar_num_nodes(){
	fseek(arquivo_arvoreB, sizeof(int), SEEK_SET);
	fwrite(&num_nodes, sizeof(int), 1, arquivo_arvoreB);
}

/*################################################################################*/

void salvar_num_regs(){
	fseek(arquivo_arvoreB, 2*sizeof(int), SEEK_SET);
	fwrite(&num_regs_armazenados, sizeof(int), 1, arquivo_arvoreB);
}

/*################################################################################*/

registro_B criar_registro(char nome[20], int chave, int idade){
	registro_B _reg;

	_reg.chave = chave;
	strcpy(_reg.nome, nome);
	_reg.idade = idade;

	return _reg;

}

/*################################################################################*/

int reg_posicao(int chave, node temp){
	int i=0;

	//2*ordem-1
	while(  (i < (2*ORDEM) ) && (chave > temp.registro[i].chave)  && (temp.registro[i].chave!=-1))
			i++;

	if(i==(2*ORDEM))
		i--;

	return i;
}

/*################################################################################*/

registro_B organizar_registros(node *b_node, int indice, registro_B b_reg){
	int i;
	registro_B temp;


	if(indice <= (2*ORDEM)-1){
		if(b_reg.chave!=-1){
			if(indice!=(2*ORDEM)-1){
				for(i=indice; i < 2*ORDEM; i++){
					temp = b_node->registro[i];
					b_node->registro[i] = b_reg;
					b_reg = temp;
				}
			}
			else
				if(b_node->registro[indice].chave==-1)
					b_node->registro[indice] = b_reg;
		}
		else
		{	//remover
			if(indice!= (2*ORDEM)-1 )
				for(i=indice; i < (2*ORDEM)-1; i++)
					b_node->registro[i] = b_node->registro[i+1];

			b_node->registro[2*ORDEM-1] = b_reg;
		}
	}

	//retornando ultimo elemento na sequencia. no caso do overflow,
	//ultimo elemento, o qual nao cabe na sequencia
	return b_reg;
}

/*################################################################################*/

int organizar_ponteiros(node *b_node, int indice, int ponteiro_filho){
	int i, aux;


	if(ponteiro_filho!=0){
		for(i=indice; i < 2*ORDEM+1; i++){
			aux = b_node->filhos[i];
			b_node->filhos[i] = ponteiro_filho;
			ponteiro_filho = aux;
		}
	}
	else
	{	//remover
		if(indice!=2*ORDEM)
			for(i=indice; i < 2*ORDEM; i++)
				b_node->filhos[i] = b_node->filhos[i+1];

		b_node->filhos[2*ORDEM] = ponteiro_filho;
	}

	return ponteiro_filho;
}

/*################################################################################*/

void split_node(node b_node, int indice, registro_B *half_reg, registro_B overflow_reg){
	int i;


	if(indice!=ORDEM){
		for(i=0; i < ORDEM; i++)
			half_reg[i] = b_node.registro[i];
	}
	else
	{
		for(i=indice+1; i < 2*ORDEM; i++)
			half_reg[i - ORDEM - 1] = b_node.registro[i];

		half_reg[ORDEM-1] = overflow_reg;
	}
}

/*################################################################################*/

void inserir(char nome[20], int chave, int idade, int indice, int elem_cmp, int pai){
	int i=0;
	node arv_b_node;
	registro_B reg_inserido, *aux=NULL;


	ler_No(&arv_b_node, indice);

	//no folha
	if(arv_b_node.filhos[0]==0){
		i = reg_posicao(chave, arv_b_node);

		//nao insere registro repetido
		if(arv_b_node.registro[i].chave==chave){
			printf("chave ja existente: %d", chave);
			return;
		}

		//nao insere no No ainda!
		reg_inserido = criar_registro(nome, chave, idade);


		//uma posicao vazia
		if(arv_b_node.registro[i].chave==-1){
			arv_b_node.registro[i] = reg_inserido;
			salvar_No(&arv_b_node, indice);
		}
		else
		{//posicao nao vazia e/ou overflow
			aux = (registro_B*) malloc(sizeof(registro_B));
			*aux = organizar_registros(&arv_b_node, i, reg_inserido);
			//linha acima:retorna ultimo elem da sequencia de registro(!=-1 se overflow)
			salvar_No(&arv_b_node, indice);

			//overflow (aux->chave tem valor -1 caso nao tenha overflow)
			if(aux->chave!=-1)
				b_tree_split(elem_cmp, indice, arv_b_node.registro[ORDEM], pai, *aux);

			free(aux);
		}

		num_regs_armazenados++;
		salvar_num_regs();

	}//fim: no folha
	else
	{//pagina nao e folha

		i = reg_posicao(chave, arv_b_node);

		if(arv_b_node.registro[i].chave==chave){
			printf("chave ja existente: %d", chave);
			return;
		}

		if(chave < arv_b_node.registro[i].chave){
			//vai pro lado esq
			inserir(nome, chave, idade, arv_b_node.filhos[i], i, indice);
		}
		else
		{
			//vai pra direita
			inserir(nome, chave, idade, arv_b_node.filhos[i+1], i, indice);
		}

	}//else:pagina nao e folha

}

/*################################################################################*/

void b_tree_split(int ind_elem_cmp, int indice, registro_B _elementoDoMeio, int pai, registro_B overflow){
	int i, indice_pai, esq_filhos[ORDEM+1], dir_filhos[ORDEM+1], raiz_filhos[2*ORDEM+1], indice_node_esq, indice_node_dir;
	node node_aux;
	registro_B reg_aux, temp_esq[ORDEM], temp_dir[ORDEM];


	ler_No(&node_aux, indice);
	split_node(node_aux, 0, temp_esq, overflow);
	split_node(node_aux, ORDEM, temp_dir, overflow);
	inicializar_filhos(esq_filhos, dir_filhos);

	reg_aux = criar_registro("\0", -1, -1);


	for(i=0; i < 2*ORDEM+1; i++)
		raiz_filhos[i] = node_aux.filhos[i];

	if(indice==1){
		inicializar_no(&node_aux);
		indice_pai = indice;
		indice_node_esq = prox_no;
		indice_node_dir = prox_no+1;


		node_aux.filhos[0] = prox_no;
		node_aux.filhos[1] = prox_no+1;

		if(raiz_filhos[0]!=0){
			for(i=0; i <= ORDEM; i++){
				esq_filhos[i] = raiz_filhos[i];
				dir_filhos[i] = raiz_filhos[ORDEM+1+i];
			}

			dir_filhos[ORDEM] = ultimo_no_inserido;

		}

		reg_aux = organizar_registros(&node_aux, 0, _elementoDoMeio);


	}
	else
	{	//indice!=1
		indice_pai = pai;
		indice_node_esq = indice;
		indice_node_dir = prox_no;
		ultimo_no_inserido = prox_no;

		ler_No(&node_aux, pai);
		reg_aux = organizar_registros(&node_aux, ind_elem_cmp, _elementoDoMeio);

		if(ind_elem_cmp!=2*ORDEM-1)
			ultimo_no_inserido = organizar_ponteiros(&node_aux, ind_elem_cmp+1, prox_no);
		else
			ultimo_no_inserido = prox_no;

	}

	salvar_No(&node_aux, indice_pai);
	inicializar_no(&node_aux);

	for(i=0; i < ORDEM; i++){
		node_aux.registro[i]= temp_esq[i];
		node_aux.filhos[i] = esq_filhos[i];
	}
		node_aux.filhos[ORDEM] = esq_filhos[ORDEM];

	salvar_No(&node_aux, indice_node_esq);

	for(i=0; i < ORDEM; i++){
		node_aux.registro[i]= temp_dir[i];
		node_aux.filhos[i] = dir_filhos[i];
	}
		node_aux.filhos[ORDEM] = dir_filhos[ORDEM];

	salvar_No(&node_aux, indice_node_dir);


	if(indice==1){
		prox_no+=2;
		num_nodes+=2;
	}
	else
	{
		prox_no++;
		num_nodes++;
	}

	salvar_num_nodes();

	if(prox_no!=num_nodes+1)
		ajustar_prox_pos_vazia();
	else
		salvar_pos_vazia();




	if(reg_aux.chave!=-1){
		ler_No(&node_aux, pai);
		consultar(node_aux.registro[0].chave, 1, 'r', &pai);
		b_tree_split(ind_elem_cmp, indice_pai, node_aux.registro[ORDEM], pai, reg_aux);
	}

}

/*################################################################################*/

void imprimir(int indice){
	int i=0, altura, num_no=1;

	//a altura e menor igual a: log (numero de nodes +1 /2) na base (ORDEM+1) + 1
	//usando log na base 10:
	if(num_regs_armazenados<= (2*ORDEM) )
		altura = 1;
	else
		altura = ceil( log10(num_regs_armazenados+1/2)/log10(ORDEM+1) );

	for(i=1; i <= altura; i++)
		imprimir_level(indice, i, &num_no);

}

/*################################################################################*/

void imprimir_level(int indice, int level, int *n_node){
	int i, num_regs;
	node node_print;


	ler_No(&node_print, indice);

	if(level==1){
		printf("No: %d apontador: ", *n_node);
		num_regs = utilizacao_node(&node_print);
		for(i=0; i < num_regs+1; i++)
			if(node_print.filhos[i]!=0)
				printf("%d ", node_print.filhos[i]);
			else
				printf("nulo ");

		printf("chave: ");
		for(i=0; i < num_regs; i++)
			printf("%d ",node_print.registro[i].chave);

		printf("\n");
		(*n_node)++;

	}
	else if (level > 1){

		for(i=0; i < 2*ORDEM+1; i++){
			if(node_print.filhos[i]==0)
				break;
			imprimir_level(node_print.filhos[i], level-1, n_node);
		}
	}

}

/*################################################################################*/

void print_ordem_crescente(int indice){
	node temp;
	int i=0;


	ler_No(&temp, indice);

	//apenas as chaves
	for(i=0; i < (2*ORDEM)+1; i++)
		if(temp.filhos[i]!=0){
			print_ordem_crescente(temp.filhos[i]);
			if((i<2*ORDEM) && (temp.registro[i].chave!=-1))
			printf("%d\n", temp.registro[i].chave);
		}
		else
			break;


	if(temp.filhos[0]==0)
		for(i=0; i < 2*ORDEM; i++){
			if(temp.registro[i].chave==-1)
				break;
			printf("%d\n", temp.registro[i].chave);
		}
}

/*################################################################################*/

int consultar(int chave, int indice, char op, int *pai){
	node temp;
	int i, n;


	//tambem retorna o indice do pai!

	ler_No(&temp, indice);
	i = reg_posicao(chave, temp);


	if(  ( (temp.registro[i].chave==-1) && (temp.filhos[i]==0) )
	|| ( (chave < temp.registro[i].chave) && (temp.filhos[i]==0) )
	|| ( (chave > temp.registro[i].chave) && (temp.filhos[i]==0) )  )
	{
		printf("chave nao encontrada: %d", chave);
		return 0;
	}

	if(temp.registro[i].chave==chave){
		if(op=='c')
			printf("chave: %d\n%s\n%d", temp.registro[i].chave, temp.registro[i].nome, temp.registro[i].idade);
		//new
		if(indice==1)
			*pai = 0;
		return indice;
	}
	else
	{
		*pai = indice;
		if(chave < temp.registro[i].chave)
			n = consultar(chave, temp.filhos[i], op, pai);
		else
			n = consultar(chave, temp.filhos[i+1], op, pai);
	}

	return n;
}

/*################################################################################*/

void remover(int chave){
	node temp;
	registro_B reg_aux;
	int indice, i=0, indice_aux, irmao, pai;

	indice = consultar(chave, 1, 'r', &pai);

	if(indice==0)
		return;


	ler_No(&temp, indice);
	indice_aux = indice;

	i = reg_posicao(chave, temp);

	remover_posicao(i, &temp, indice, -1, -1);

	//No nao e folha, pegar o sucessor e mover pra posicao deletada
	if(temp.filhos[i+1]!=0){
		pai = indice_aux;
		indice_aux = temp.filhos[i+1];

		ler_No(&temp, temp.filhos[i+1]);

		//pegar sucessor
		while(temp.filhos[0]!=0){
			pai = indice_aux;
			indice_aux = temp.filhos[0];
			ler_No(&temp, temp.filhos[0]);
		}

		reg_aux = temp.registro[0];
		remover_posicao(0, &temp, indice_aux, -1, -1);

		//salvar no NO pai
		ler_No(&temp, indice);
		organizar_registros(&temp, i, reg_aux);
		salvar_No(&temp, indice);


	}

	ler_No(&temp, indice_aux);


	while((utilizacao_node(&temp) <= (ORDEM-1)) && (pai!=0) ){
		irmao = selecionar_irmao(pai, indice_aux);
		juntar_no(indice_aux, irmao, pai);
		indice_aux = pai;
		ler_No(&temp, indice_aux);
		consultar(temp.registro[0].chave, 1, 'r', &pai);	//pegar o pai


	}

	num_regs_armazenados--;
	salvar_num_regs();

}

/*################################################################################*/

void remover_posicao(int indice, node *temp, int pos_arquivo, int ponteiro, int ind_filho_removido){


	temp->registro[indice] = criar_registro("\0", -1, -1);
	organizar_registros(temp, indice, temp->registro[indice]);

	//ponteiro = -1  (=>) nao necessario organizar
	if(ponteiro==0)
		organizar_ponteiros(temp, ind_filho_removido, ponteiro);
	else
		if(ponteiro!=-1)
			organizar_ponteiros(temp, indice, ponteiro);

	salvar_No(temp, pos_arquivo);

}

/*################################################################################*/

void juntar_no(int indice, int irmao, int pai){
	node temp;
	int i, ind=0, num_registros, num_apontadores, ind_aux, apontadores[2*ORDEM+1], primeiroFilho, segundoFilho, pt_aux;
	registro_B reg_copy[ORDEM], reg_cmp, reg_aux;



	for(i=0; i < ORDEM; i++)
		reg_copy[i] = criar_registro("\0", -1, -1);

	for(i=0; i < (2*ORDEM)+1; i++)
		apontadores[i] = 0;

	//pegar elem comparado
	ler_No(&temp, pai);
	num_apontadores = num_filhos(&temp);

	//No que foi movido o elemento
	while((ind < (2*ORDEM)+1) && (temp.filhos[ind]!=indice) && (temp.filhos[ind]!=irmao))
		ind++;

	if(temp.registro[ind].chave==-1 || (ind==2*ORDEM+1))
		ind--;

	//descobrir a ordem dos Nos, segundo seus registros
	if(temp.filhos[ind]==indice){
		primeiroFilho = indice;
		segundoFilho = irmao;
		pt_aux = 0;
	}
	else
		if(temp.filhos[ind]==irmao){
			primeiroFilho = irmao;
			segundoFilho = indice;
			pt_aux = 1;
		}


	reg_cmp = temp.registro[ind];	//No Pai, da comparacao

	//indice do split e o irmao
	ler_No(&temp, irmao);

	//Ajustar Nos: (um node e removido, tem 'ordem' registros)
	if((num_registros = utilizacao_node(&temp))==ORDEM){
		ler_No(&temp, pai);

		if(temp.filhos[ind]==irmao)
			remover_posicao(ind, &temp, pai, 0, ind);
		else
			remover_posicao(ind, &temp, pai, 0, ind+1);


		ler_No(&temp, irmao);
		for(i=0; i < ORDEM; i++)
			reg_copy[i] = temp.registro[i];

		//pegar os apontadores,
		if(temp.filhos[0]!=0)
			for(i=0; i < ORDEM; i++)
				apontadores[i] = temp.filhos[i];


		ler_No(&temp, indice);
		organizar_registros(&temp, reg_posicao(reg_cmp.chave, temp), reg_cmp);

		for(i=ORDEM; i < 2*ORDEM; i++)
			organizar_registros(&temp, reg_posicao(reg_copy[i - ORDEM].chave, temp), reg_copy[i - ORDEM] );

		//if(pai!=1){
		if(num_apontadores!=2){
			salvar_No(&temp, indice);
			ler_No(&temp, primeiroFilho);
			if(temp.filhos[0]!=0){
				for(i=0; i < ORDEM + pt_aux; i++)
					apontadores[i] = temp.filhos[i];

				ler_No(&temp, segundoFilho);
				for(i = ORDEM + pt_aux; i < 2*ORDEM+1; i++)
					apontadores[i] = temp.filhos[i - ORDEM - pt_aux];

				ler_No(&temp, indice);
				for(i=0; i < 2*ORDEM+1; i++)
					temp.filhos[i] = apontadores[i];

				salvar_No(&temp, indice);
				num_nodes--;
			}

		}
		else
		{
			salvar_No(&temp, pai);
			ler_No(&temp, primeiroFilho);
			if(temp.filhos[0]!=0){
				for(i=0; i < ORDEM + pt_aux; i++)
					apontadores[i] = temp.filhos[i];

				ler_No(&temp, segundoFilho);
				for(i = ORDEM + pt_aux; i < 2*ORDEM+1; i++)
					apontadores[i] = temp.filhos[i - ORDEM - pt_aux];

				ler_No(&temp, pai);
				for(i=0; i < 2*ORDEM+1; i++)
					temp.filhos[i] = apontadores[i];

				num_nodes-=2;
				salvar_No(&temp, pai);
			}
		}

		deletar_No(irmao);
		salvar_pos_vazia();
		salvar_num_nodes();

	}
	else
	{
	//
	//utilizacao do irmao > ORDEM-1 (nao elimina nenhum No)
	//
		//indice do elemento que ira subir pro No pai
		ind_aux = floor( (ORDEM + num_registros)/2 );
		i = ind_aux;
		reg_aux = reg_cmp;

		//pegando o elemento que sobe pro No pai
		ler_No(&temp, primeiroFilho);

		//irmao e os segudno filho
		if(utilizacao_node(&temp) <= (ORDEM-1)){
			ler_No(&temp, segundoFilho);

			reg_cmp = temp.registro[ind_aux - ORDEM];
			apontadores[0] = temp.filhos[ind_aux - ORDEM];
			remover_posicao(ind_aux - ORDEM, &temp, segundoFilho, 0, ind);

			i = i - ORDEM - 1;

			while((i >= 0) && (temp.registro[i].chave!=-1)){
				reg_copy[i - (ind_aux - ORDEM - 1)] = temp.registro[i];
				temp.registro[i] = criar_registro("\0", -1, -1);
				organizar_registros(&temp, i, temp.registro[i]);
				if(temp.filhos[i]!=0)
					apontadores[ind_aux - i + 1] = temp.filhos[i];
				i--;
			}
			salvar_No(&temp, segundoFilho);
		}
		else
		{
			reg_cmp = temp.registro[ind_aux];
			apontadores[0] = temp.filhos[ind_aux];
			remover_posicao(ind_aux, &temp, primeiroFilho, -1, ind);

			while((i < (2*ORDEM)) && (temp.registro[i].chave!=-1)){
				reg_copy[i - ind_aux] = temp.registro[i];
				temp.registro[i] = criar_registro("\0", -1, -1);
				organizar_registros(&temp, i, temp.registro[i]);
				if(temp.filhos[i]!=0)
					apontadores[i- ind_aux + 1] = temp.filhos[i];
				i++;
			}
			salvar_No(&temp, primeiroFilho);
		}


		//salvar no pai o novo registro pai
		ler_No(&temp, pai);
		temp.registro[ind] = reg_cmp;
		salvar_No(&temp, pai);

		ler_No(&temp, indice);
		i = reg_posicao(reg_aux.chave, temp);
		organizar_registros(&temp, i, reg_aux);
		if(apontadores[0]!=0)
			organizar_ponteiros(&temp, i+1, apontadores[0]);
		i=0;
		while(reg_copy[i].chave!=-1){
			organizar_registros(&temp, reg_posicao(reg_copy[i].chave, temp), reg_copy[i]);
			organizar_ponteiros(&temp, i+2, apontadores[i+1]);
			i++;
		}

		salvar_No(&temp, indice);

	}

}

/*################################################################################*/

int selecionar_irmao(int pai, int filho_reg_remov){
	int i=0, esq, dir;
	node temp;


	ler_No(&temp, pai);

	while(temp.filhos[i]!=filho_reg_remov)
		i++;

	if(i>0)
		esq = temp.filhos[i-1];

	dir = temp.filhos[i+1];

	if(i>0){
		ler_No(&temp, esq);
		if(utilizacao_node(&temp) >= (ORDEM-1))
			return esq;
	}

	ler_No(&temp, dir);
	if(utilizacao_node(&temp) >= (ORDEM-1))
			return dir;

	return -1;

}

/*################################################################################*/

int utilizacao_node(node *temp){
	int i=0;

	//retorna o numero de registros no Node
	while( (temp->registro[i].chave!=-1) && (i < 2*ORDEM) )
		i++;

	return i;
}

/*################################################################################*/

int num_filhos(node *temp){
	int i = 0;

	while(temp->filhos[i]!=0)
		i++;

	return i;
}

/*################################################################################*/

void deletar_No(int indice){
	node temp;

	inicializar_no(&temp);
	salvar_No(&temp, indice);
	prox_no = indice;

	salvar_pos_vazia();
	salvar_num_nodes();
}
/*################################################################################*/
