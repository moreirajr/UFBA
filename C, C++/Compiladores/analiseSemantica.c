#include "analiseSemantica.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>


TabelaSimbolos *tabelaDeSimbolos=NULL, *tabela_aux=NULL;

//int expressaoAritMetica (ArvoreSintatica *arv,int contexto){
//    int i = 0;
//	if(arv->node.token <21){
//
//		 for(i=0; i < arv->n_filhos; i++){
//			 expressaoAritMetica(&arv->filhos[i],contexto);
//		 }
//
//   }else{
//	   TabelaLookUp(arv->node, contexto, 0, 0, 0);
//   }
//
//	return 0;
//}


int expressaoAritmetica (ArvoreSintatica *arv,int contexto){

	int ret = 1;

	    if(arv!=NULL){
	        int i;
	        if(arv->node.token < 21){ //nao terminal
	            if(arv->node.token == NT_ID){
	                if(! TabelaLookUp(arv->filhos[0].node, contexto, 0, 0, 0) )
	                    return 0;
	                if( (tabela_aux->vetor) && (arv->filhos[1].n_filhos!=3) )
	                    return 0;
	                if( !(tabela_aux->vetor) && (arv->filhos[1].n_filhos==3) )
	                	return 0;
	            }
	            else
	                for(i=0; i < arv->n_filhos; i++)
	                    ret = ret * expressaoAritmetica(&arv->filhos[i], contexto);
	        }

	    }

	    return ret;
}
int analiseSemantica(ArvoreSintatica *arv, int contexto){
	int ret = 1;

	//printf("\n\n\n\nInicio da analise semantica no contexto %d", contexto);

	if(arv!=NULL){
		ArvoreSintatica *aux;
		//printf("\nTem %d filhos", arv->n_filhos);

		if( (arv->node.token == C) && (arv->n_filhos > 0) ){// C -> CMD C, nao entra se C -> vazio
			ret = analiseSemantica(&arv->filhos[0], contexto);
			ret = ret * analiseSemantica(&arv->filhos[1], contexto);
		}
		else{

			if(arv->node.token == CMD){
				if(arv->filhos[0].node.token == DECLARE){
					int vet, tipo;
					aux = &arv->filhos[3]; //Tipo
					tipo = aux->filhos[0].node.token;
					aux = &arv->filhos[1]; //token L (lista de Ids)


					do{ //printf("\n--DO--");
						if(aux->node.token == L2)
							aux = &aux->filhos[1];
						if( aux->filhos[1].n_filhos == 2) //vetor
							vet = 1;
						else
							vet = 0;
					//	printf("\n%s", vet==1 ? "Vetor" : "Nao vetor");
						if(TabelaInsert(aux->filhos[0].node, contexto, vet, tipo) ){// printf("\nRetorna 0");
							return 0;
						}
						aux = &aux->filhos[2];
						//printf("\naux tem %d filhos", aux->n_filhos);
					} while( aux->n_filhos != 0); //L' com um unico filho (vazio)
				}
				else if(arv->filhos[0].node.token == RESIZE){
					//printf("\nResize");
					aux = &arv->filhos[1];
					if(! TabelaLookUp(aux->filhos[0].node, contexto, 0, 0, 0) ){//	printf("\nRetorna 0");
						return 0;
					}
					else{
					//	printf("\nElse");
						if(!tabela_aux->vetor) //alocando variavel nao vetor
							return 0;
						tabela_aux->alocado = 1;
					}
				}
				else if(arv->filhos[0].node.token == PUT){
					aux = &arv->filhos[1]; //Q da prod cmd -> put Q
					if(aux->filhos[0].node.token == CSTRING){ // Q -> string in id..
						if(! TabelaLookUp(aux->filhos[2].node, contexto, 0, 0, 0) )
							return 0;	//procurar o id, que e armazenado em tabela_aux
						if( (tabela_aux->tipo != LETTER) || (!tabela_aux->vetor) || (!tabela_aux->alocado))
							return 0;
					}
					else if(aux->filhos[0].node.token == E) {//  Q -> E in ID gdf
						ret = expressaoAritmetica(&aux->filhos[0],contexto);
						if(ret == 1){
							ret = expressaoAritmetica(&aux->filhos[2],contexto);
						}
					}
				}
				else if(arv->filhos[0].node.token == IF){
					//passando o node C -> CMD C (tanto no if quanto no else)
					if(arv->filhos[4].n_filhos > 0) //If possui comandos
						ret = analiseSemantica(&arv->filhos[4], contexto+1);
					if(arv->filhos[6].n_filhos > 0){ //else possui comandos
						aux = &arv->filhos[6];    //CMD -> if exp_r then [C] ELSE e ELSE -> else [ C ]
						ret = analiseSemantica(&aux->filhos[2], contexto+1);
					}
				}
				else if(arv->filhos[0].node.token == FOR){
					//ver se filho 1 e vetor e esta indexado ou variavel comum
					aux = &arv->filhos[1];
					if(!TabelaLookUp(aux->filhos[0].node, contexto, 0, 0, 0) ){
						return 0;
					}
					if( (tabela_aux->vetor) && (aux->filhos[1].n_filhos != 2)){
						return 0;	//vetor nao indexado
					}
					if( (tabela_aux->vetor) && (!tabela_aux->alocado)){
						return 0;
					}
					if(expressaoAritmetica(&arv->filhos[3], contexto)==0){
						return 0;
					}
					if(expressaoAritmetica(&arv->filhos[5], contexto)==0){
						return 0;
					}
					//tratar variavel que nao e vetor sendo indexada
					if(arv->filhos[8].n_filhos > 0){	//outro comando, node C
						ret = analiseSemantica(&arv->filhos[8], contexto+1);
					}
				}
				else if(arv->filhos[0].node.token == FOREACH){
					//ver se filho 1 e vetor e esta indexado ou variavel comum
					//ver se filho 3 e vetor (sem [])
					aux = &arv->filhos[1];
					if(! TabelaLookUp(aux->filhos[0].node, contexto, 0, 0, 0) )
						return 0;
					if( (tabela_aux->vetor) && (aux->filhos[1].n_filhos != 2))
						return 0;	//vetor nao indexado
					if( (tabela_aux->vetor) && (!tabela_aux->alocado))
						return 0;
					if(! TabelaLookUp(arv->filhos[3].node, contexto, 0, 0, 0) )
						return 0;
					if(arv->filhos[6].n_filhos > 0)	//outro comando, node C
						ret = analiseSemantica(&arv->filhos[6], contexto+1);
				}
				else if(arv->filhos[0].node.token == READ){
					aux = &arv->filhos[1];
					if(!TabelaLookUp(aux->filhos[0].node, contexto, 0, 0, 0) )
						return 0;
			    	if( (tabela_aux->vetor) && (!tabela_aux->alocado))
							return 0;
					if( (!tabela_aux->vetor) && (aux->filhos[1].n_filhos==3) )
							return 0;
				}
				else if(arv->filhos[0].node.token == PRINT){
					aux = &arv->filhos[1];
					aux = &aux->filhos[0];
					if(!TabelaLookUp(aux->filhos[0].node, contexto, 0, 0, 0) )
						return 0;
					if( (tabela_aux->vetor) && (!tabela_aux->alocado))
						return 0;
					if( (!tabela_aux->vetor) && (aux->filhos[1].n_filhos==3) )
						return 0;
				}
			}//token == CMD
		}//else
	}//arv!=null

	return ret;
}

/*************************************************************/

int TabelaLookUp(Token s_token, int contexto, int op, int vet, int tipo){
	TabelaSimbolos *aux, *ant=NULL;

//	printf("\nLOOKUP procurando [%s] no contexto %d", s_token.nome, contexto);
	aux = &tabelaDeSimbolos[contexto];

	if( (contexto>0)){	//declarada no contexto 0 (global)
		if(TabelaLookUp(s_token, 0, 0, vet, tipo))
			return 1;
	}

	if(tabelaDeSimbolos==NULL || ( contexto > sizeof(tabelaDeSimbolos)/sizeof(TabelaSimbolos)  ) ){
		if(op){
			tabelaDeSimbolos = (TabelaSimbolos*) realloc (tabelaDeSimbolos, (contexto+1) * sizeof(TabelaSimbolos) );
			tabelaDeSimbolos[contexto].token = s_token;
			tabelaDeSimbolos[contexto].vetor = vet;
			tabelaDeSimbolos[contexto].alocado = 0;
			tabelaDeSimbolos[contexto].tipo = tipo;
			tabelaDeSimbolos[contexto].next = NULL;
		}
		return 0;
	}



	while(aux!=NULL){
		if(!strcmp(aux->token.nome, s_token.nome) ){
		//	printf("\n{Token %s}", s_token.nome);
			tabela_aux = aux;
			return 1;
		}
		ant = aux;
		aux = aux->next;
	}

	if(op){
		TabelaSimbolos *new = (TabelaSimbolos*) malloc(sizeof(TabelaSimbolos));
		new->token = s_token;
		new->vetor = vet;
		new->alocado = 0;
		new->tipo = tipo;
		new->next = NULL;
		ant->next = new;
	}


	return 0;
}

/*************************************************************/

int TabelaInsert(Token s_token, int contexto, int vet, int tipo){
	//printf("\nINSERT {token %d}", s_token.token);
	return TabelaLookUp(s_token, contexto, 1, vet, tipo);
}

/*************************************************************/

void TabelaRemove(TabelaSimbolos *tab, int contexto){
	TabelaSimbolos *aux, *temp;

	aux = tab[contexto].next;

	while (aux!=NULL){
		temp = aux;
		free(temp);
		aux = aux->next;
	}

}

/*************************************************************/

