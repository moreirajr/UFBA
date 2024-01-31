#ifndef DEFINICOES_H_
#define DEFINICOES_H_

//Aluno: Evaldo M. Moreira Junior

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 2
#define ARQUIVO "registroArvoreB.bin"

FILE *arquivo_arvoreB;


typedef struct REG {
	unsigned int chave;
	char nome[20];
	unsigned int idade;
} registro_B;


typedef struct NO{
	registro_B registro[2*ORDEM];
	int filhos[ (2*ORDEM) + 1];
} node;


node no;
int prox_no, ultimo_no_inserido, num_nodes, num_regs_armazenados;



void inicializar(node *temp);
void inicializar_no(node *temp);
void inicializar_filhos(int *filho_1, int *filho_2);
void criar_arquivo();
registro_B criar_registro(char nome[20], int chave, int idade);

void salvar_No(node *b_node, int pos_arquivo);
void ler_No(node *b_node, int pos_arquivo);
void salvar_pos_vazia();
void ajustar_prox_pos_vazia();
void salvar_num_nodes();
void salvar_num_regs();

int reg_posicao(int chave, node temp);

void inserir(char nome[20], int chave, int idade, int indice, int elem_cmp, int pai);
registro_B organizar_registros(node *b_node, int indice, registro_B b_reg);
int organizar_ponteiros(node *b_node, int indice, int ponteiro_filho);
void split_node(node b_node, int indice, registro_B *half_reg, registro_B overflow_reg);
void b_tree_split(int ind_elem_cmp, int indice, registro_B _elementoDoMeio, int pai, registro_B overflow);

int consultar(int chave, int indice, char op, int *pai);
void remover(int chave);
void remover_posicao(int indice, node *temp, int pos_arquivo, int ponteiro, int ind_filho_removido);
void deletar_No(int indice);

void juntar_no(int indice, int irmao, int pai);
int selecionar_irmao(int pai, int filho_reg_remov);
int utilizacao_node(node *temp);
int num_filhos(node *temp);

void imprimir(int indice);
void imprimir_level(int indice, int level, int *n_node);
void print_ordem_crescente(int indice);


#endif /* DEFINICOES_H_*/
