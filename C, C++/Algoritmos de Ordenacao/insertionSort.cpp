#include <iostream>
#include <cstdlib>
#include <ctime>



using namespace std;

void insertionSort(int n, int vetor[]);

int main(int argc, char *argv[]) {
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

	insertionSort(n, vetor);

	return 0;
}

//http://pt.wikipedia.org/wiki/Algoritmo_de_ordena%C3%A7%C3%A3o

void insertionSort(int n, int vetor[]){
	int i, j, chave;

	for(i=1; i < n; i++)
		chave = vetor[i];

}
