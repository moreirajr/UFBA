#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opGeometricas.h"
#include "triangulacao.h"

void initVertices() {
	
/*
	Para cada vertice da linha poligonal fechada faca
		Se o vertice e seus vizinhos anteriores formam uma diagonal entao
      		Ž orelha;
      	caso contrario
      		nao Ž orelha; 
*/     						
} 

void Triangular() {
/* 
	Se o vertice corrente Ž orelha entao
		cria um triangulo da triangulacao com o vertices e seus vizinhos anterior e posterior;
			remover o vertice da poligonal;
			reclassificar seus vizinhos anterior e posterior se formam um nova orelha;
*/
}

bool	eDiagonal(/* a, b */) {
	
/* 
	Se os pontos (a, a->proximo, a->anterior, b) formam uma diagonal interna
		E os pontos (b, b->proximo, b->anterior, b) formam uma diagonal interna
		E se a diagonal (a,b) e propria


		Utilizar a rotinas:
		bool	InCone( tPonto a, tPonto a1, tPonto a0, tPonto b );
		bool   eDiagonalPropria();

*/
}

bool   eDiagonalPropria( /* a, b */ ) {
	
/* 	Para cada vertice c da poligonal 
		c1 = proximo vertice de c;
		se (a aresta (c,c1) nao Ž incidente a (a,b) ) e 
		   ( existe intersecao entre as arestas (a,b) e (c, c1))
		   retornar falso => diagonal nao valida;
	fim-para
	retornar verdadeiro => e diagonal valida;
	
	Utilizar a rotina: 
	bool	Intersect( tPonto a, tPonto b, tPonto c, tPonto d );

	
*/

}



