#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_ARQUIVO 11			//tamanho da tabela hash
#define ARQUIVO "registro.bin"		//nome do arquivo

FILE *reg_hash;

typedef struct REG {
	unsigned int chave;
	char nome[20];
	unsigned int idade;
} registro;

typedef struct ARV{
	unsigned int indice;
	unsigned int chave;
	struct ARV *filho_esq, *filho_dir, *pai;
} arvore;


registro tabela_hash[TAMANHO_ARQUIVO];
unsigned int *controle, idade;
char nome[20];


void inicializar();
void carregar_arquivo();
void salvar_arquivo();

unsigned int h1(unsigned int chave);
unsigned int h2(unsigned int chave);

void inserir(char nome[20], unsigned int chave, unsigned int idade, int OP);
int consultar(unsigned int chave, int OP);
void imprimir();
void remover(unsigned int indice);
void tentativa_insercao(unsigned int indice);
void media_acessos();
arvore* criar_arvore(arvore *tree, arvore *pai, unsigned int chave, unsigned int incremento, int indice,  int level, int mover, int OP);
arvore *criar_no(arvore *node, arvore *pai, unsigned int indice, unsigned int chave);
void mover_no(arvore* tree, int mover);
void trocar_valores(int a, int b);

#endif /* DEF_H_ */
