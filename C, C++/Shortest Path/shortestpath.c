#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]){
	int i, j, n, m, *distancia,
	*vetor_B, *vetor_S, *vetor_T;



	printf("\nValor de n: (pilares) e valor de m (pontes):\n\n");
	scanf("%d %d", &n, &m);

	vetor_B = (int*)malloc(m*sizeof(int));
	vetor_S = (int*)malloc(m*sizeof(int));
	vetor_T = (int*)malloc(m*sizeof(int));
	distancia = (int*)malloc( (n+2)*sizeof(int) );


	//ler dados
	for(i=0; i < m; i++)
		scanf("%d %d %d", &vetor_S[i], &vetor_T[i], &vetor_B[i]);

	//print resultado
	printf("\nInseido:\n");
	for(i=0; i < m; i++)
		printf("%d %d %d\n", vetor_S[i], vetor_T[i], vetor_B[i]);


	//inicializar distancias
	for(i=0; i < n+2; i++)
		distancia[i] = 999;

	//distancias iniciais, do inicio (zero) ate vetor_T[i]
	for(i=0; i < m; i++)
		if(vetor_S[i]==0)
			distancia[ vetor_T[i] ] = vetor_B[i];


	printf("\nDISTANCIAS ::: ");
	for(i=0; i < n+2; i++)
		printf("%d ", distancia[i]);




	for(j=1; j < n+2; j++)
		for(i=0; i < m; i++)
			if(vetor_T[i]==j){
				if(distancia[ vetor_S[i] ] + vetor_B[i] < distancia[ vetor_T[i] ])
					distancia[ vetor_T[i] ] = distancia[ vetor_S[i] ] + vetor_B[i];
			}


	printf("\n\nResultado: ");
		for(i=0; i < n+2; i++)
			printf("%d ", distancia[i]);










	system("pause");
	return 0;
}
