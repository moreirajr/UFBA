#ifndef DEF_H_
#define DEF_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define N 2	//numero de paginas inicial
#define REGISTROS_POR_PAGINA 3	//mudar pra 3!!!
#define UTILIZACAO_MIN 40
#define UTILIZACAO_MAX 80
#define ARQUIVO1 "registro.bin"
#define ARQUIVO2 "info.txt"
#define ARQUIVO3 "overflowpages.bin"


FILE *arquivo_hash, *arquivo_info, *arquivo_overflow;



typedef struct nomes{
	char rNome[21];
} regNomes;

typedef struct page{
	int numRegInseridos;
	unsigned int chave[REGISTROS_POR_PAGINA];
	unsigned int idade[REGISTROS_POR_PAGINA];
	struct nomes nome[REGISTROS_POR_PAGINA];
	struct page *overflow;
} paginaDeRegistro;

typedef struct Hash{
	struct page pagina;
} hash_linear;

typedef struct hashinfo{
	int level;
	int next;
	int prev;	//a posicao do next na operacao anterior
	int num_paginas;
	int num_paginas_overflow;
	int numRegInseridos;
	int numRegInseridosOverflow;
} Hash_Info;

hash_linear *pagina_hash;
Hash_Info hashinfo_nln;

char nome[20];
int idade;

void inicializar();
void carregar_arquivo();
void salvar_arquivo();
void inicializar_pagina(int indice, int tipo);

int h(int chave, int level);
void split();
float armazenamento();

void imprimir();
void inserir();
void reinserir(int m, int rchave, int ridade, char rnome[21]);
void remover(int indice, int k, int tipo, int count);	//indice da pagina, k posicao do registro, 0/1 primaria/overflow, NumPag Overf

int consultar(int chave, int func_call);
void remover_registro(int chave);
void media_de_acessos();
void balancear();



#endif /* DEF_H_ */
