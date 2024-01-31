/*
 *	Aluno: Evaldo Machado Moreira Junior
 */

#ifndef TIPOS_H_
#define TIPOS_H_

typedef struct Arvore{
	char letra;
	struct Arvore *esq, *dir;
}Btree;

Btree *inserir(Btree *arv, Btree *arv_anterior, char palavra[31], int i);
void listar_palavras(Btree *arv, char palavra[31], char temp[31], int i, int OP);
void imprimir_nivel(Btree *arv, int n, int aux, int *count);
void imprimir_arvore(Btree *arv);
Btree *ordenar_arvore(Btree *arv);
void deletar_arvore(Btree *arv);
int consultar_palavra(Btree *arv, char palavra[31], int i);
Btree *remover_palavra(Btree *arv, Btree *arv_ant, char palavra[31], int i);
void sugestao_iR(Btree *arv, char palavra[31], char temp[31], int i, int j);
void sugestao_t(Btree *arv, char palavra[31], char temp[31], int i, int j);
Btree *inserir_letra(Btree *arv, Btree *arv_prox, char letra);




#endif /* TIPOS_H_ */
