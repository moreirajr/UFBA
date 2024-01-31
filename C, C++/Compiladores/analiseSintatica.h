#ifndef ANALISESINTATICA_H_
#define ANALISESINTATICA_H_


/* Gramatica: */
#define S 0
#define C 1
#define CMD 2
#define NT_ELSE 3
#define P 4
#define L 5
#define L2 6
#define V 7
#define T 8
#define NT_ID 9
#define ID2 10
#define OPERANDO 11
#define E 12
#define G 13
#define F 14
#define Y 15
#define EXP_R 16
#define R 17
#define OP1 18
#define OP2 19
#define EXP_R2 20


#define VAZIO -3
#define ENDFILE -2
#define ERRO -1

#define DECLARE 60
#define AS 61
#define PUT 22
#define IN 23
#define IF 24
#define THEN 25
#define ELSE 26
#define FOREACH 27
#define DO 28
#define FOR 29
#define FROM 30
#define TO 31
#define RESIZE 32
#define READ 33
#define PRINT 34
#define ID 35
#define NUMBER 36
#define LETTER 37
#define CCHAR 38
#define CSTRING 39
#define NUM 40
#define EQ 41
#define NEQ 42
#define LT 43
#define GT 44
#define LTE 45
#define GTE 46
#define TIMES 47
#define PERCENT 48
#define PLUS 49
#define MINUS 50
#define OVER 51
#define LPAREN 52
#define RPAREN 53
#define LBRACKET 54
#define RBRACKET 55
#define ASPASS 56
#define ASPASD 57
#define DOT 58
#define COMMA 59

#define ACC 105
#define n_estados 110
#define n_simbolos 60	// 39 terminais e 21 nao terminais
#define num_terminais 39

typedef struct Gramatica_Prod{
	int terminal_esq;
	int num_tokens_dir;
} Producoes;

//exemplo, reducao/ler/aceita/erro :  r5 = ('r', 5), s2 = ('s', 2), ('a', 0), ('e', _ERRO_)
typedef struct par {
	char acao;
	int numero;
} Par;

typedef struct Tok{
	int token;
	char *nome;
}Token;

typedef struct Arvore{
	Token node;
	struct Arvore *pai;
	struct Arvore *filhos;
	int n_filhos;
}ArvoreSintatica;


ArvoreSintatica* analiseSintatica_SLR1(Token *tokens, ArvoreSintatica *arvoreSintatica);
int indiceSimbolo(int s);



//#####################
//PILHA:
//#####################


typedef struct pilha{
	int elemento;
	struct pilha *prox;
} Pilha;


Pilha *p_alloc(void);
Pilha *p_free(Pilha *p);
Pilha *push(int e, Pilha *p);
Pilha *pop(Pilha *p);
int top(Pilha *p);

void imprimirPilha(Pilha *p);
void imprimirArvore(ArvoreSintatica *arv);


#endif /* ANALISESINTATICA_H_ */
