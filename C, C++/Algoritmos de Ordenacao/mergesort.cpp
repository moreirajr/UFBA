#include <iostream>
#include <cstdlib>
#include <ctime>


void intercala(int p, int q, int r, int vetor[]);
void mergesort(int p, int q, int vetor[]);

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

		mergesort(0, n-1, vetor);

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

/************************************************************************************/

void intercala(int p, int q, int r, int vetor[]){
	int i, j, k, *vec;

	vec = new int[r-p];
	i = p;
	j = q;
	k = 0;

	while((i < q)&& (j < r)){
		if(vetor[i]<=vetor[j])
			vec[k++] = vetor[i++];
		else
			vec[k++] = vetor[j++];
	}

	while(i < q)
		vec[k++] = vetor[i++];
	while(j < r)
		vec[k++] = vetor[j++];

	for(i=p; i < r; i++)
		vetor[i] = vec[i-p];

	delete[] vec;

}

/************************************************************************************/

void mergesort(int p, int r, int vetor[]){
	int q;

	if(p < r-1){
		q = (p+r)/2;
		mergesort(p, q, vetor);
		mergesort(q, r, vetor);
		intercala(p, q, r, vetor);
	}
}
