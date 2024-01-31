#ifndef DEFINICOES_H_
#define DEFINICOES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_ARQUIVO 11			//tamanho da tabela hash

//arquivo utilizado: registroHash.bin
//***********************************
#define ARQUIVO "registroHash.bin"
//***********************************

FILE *arquivo_hashing;

typedef struct REG {
	unsigned int chave;
	char nome[20];
	unsigned int idade;
	unsigned int cadeia;
} registro;

unsigned int apontador_R;

registro tabela_hash[TAMANHO_ARQUIVO], tabela_h;

char metodo;


void inicializar();
void carregar_arquivo();
void criar_arquivo();
void salvar_arquivo();

unsigned int h(unsigned int chave);
void ajustar_apontador(int R);

void inserir_chave(char nome[20], int idade, int chave);
int consultar(unsigned int chave, int OP);
void imprimir();
//void remover(registro *r_cadeia, unsigned int indice_InicioCadeia, unsigned int indice);
void remover(unsigned int indice, unsigned int indice_anterior);
void remocao(unsigned int indice_atual, unsigned int indice_anterior);
void media_acessos();

#endif /* DEF_H_ */
