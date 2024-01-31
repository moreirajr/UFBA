#include <iostream>
#include <cstdlib>
#include <ctime>


void quicksort(int vetor[], int esquerda, int direita);


using namespace std;

int main(int argc, char **argv) {
	int i, n, *vetor;

	srand(time(NULL));

	cout <<"Tamanho do vetor: ";
	cin >> n;

	vetor = new int[n];

	if(vetor==0)
		cout << "Erro ao alocar vetor";

	cout <<"\nVetor: {";

	for(i=0; i < n; i++){
		vetor[i] = 1 + (rand() % 50);
		cout << vetor[i];
		if(i!=n-1)
			cout << ", ";
	}

	cout << "}";

	quicksort(vetor, 0, n-1);

	cout << "\nVetor ordenado: {";

	for(i=0; i < n; i++){
		cout << vetor[i];
			if(i!=n-1)
				cout << ", ";
	}

	cout << "}";

	cout << "\n\nPressione qualquer tecla para sair...";
	cin >> n;

	delete [] vetor;

	return 0;
}

void quicksort(int vetor[], int esquerda, int direita){
	int i, j, elementoMeio, aux;

	i = esquerda;
	j = direita;
	elementoMeio = vetor[(esquerda+direita)/2];	//valor do meio

	while(i<=j){
		while((vetor[i] < elementoMeio) && (i < direita))
			i++;
		while((vetor[j] > elementoMeio) && (j > esquerda))
			j--;

		if(i<=j){
			aux = vetor[i];
			vetor[i] = vetor[j];
			vetor[j] = aux;
			i++;
			j--;
		}

	}//end while

	if(j > esquerda)
		quicksort(vetor, esquerda, j);

	if(i < direita)
		quicksort(vetor, i, direita);
}
