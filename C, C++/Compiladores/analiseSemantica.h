#ifndef ANALISESEMANTICA_H_
#define ANALISESEMANTICA_H_

#include "analiseSintatica.h"

typedef struct TabS{
	Token token;
	struct TabS *next;
	int vetor;
	int tipo;
	int alocado;
}TabelaSimbolos;

int expressaoAritmetica (ArvoreSintatica *arv,int contexto);
void TabelaRemove(TabelaSimbolos *tab, int contexto);
int analiseSemantica(ArvoreSintatica *arv, int contexto);
int TabelaLookUp(Token s_token, int contexto, int op, int vet, int tipo);
int TabelaInsert(Token s_token, int contexto, int vet, int tipo);

#endif /* ANALISESEMANTICA_H_ */
