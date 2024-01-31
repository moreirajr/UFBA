/* Alunos:	Alvaro Andrade
 * 			Danilo Pires
 * 			Evaldo Moreira
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
#include <sstream>



#define MAX_VETOR 5 //Para as cidades adjacentes

using namespace std;

typedef struct G_cidades{
	int cidade;
	int numeroCidadesAdjacentes;	
	int cidadesAdjacentes[MAX_VETOR];	
	int distanciaReal[MAX_VETOR];	
} Grafo;


class Agente{
	Grafo *cidades;
	int numEstados, estadoIni, estadoFin, distanciaEstimada[MAX_VETOR];	
	ifstream sensor, heuristica;											

public:
	Agente(const char *filename);
	~Agente();
	int A_estrela();
	list<int> melhorCaminho(int *caminho, int estado);
	int buscaProfundidade(Grafo *cidade , int inicio);
	int buscaProfundidadeAuxiliar(Grafo *cidade , int inicio, int *nos_visitados,int contador);
	int buscaProfundidadeInicio();

};



Agente::Agente(const char *filename):sensor(filename){
	string temp;
	int valor_temp=0;
	istringstream stringSt;


	if(sensor.fail()){
		cout << "Falha ao abrir o arquivo de estados!" << endl;
	}
	else{

		getline(sensor, temp);
		this->numEstados = atoi(temp.c_str());
		this->cidades = new Grafo[this->numEstados];

		getline(sensor, temp);
		this->estadoIni = atoi(temp.c_str());

		getline(sensor, temp);
		this->estadoFin = atoi(temp.c_str());

		cout << "Numero de estados: " << this->numEstados << endl;
		cout << "Estado inicial e final: " << this->estadoIni << " e " << this->estadoFin << endl <<endl << endl;


		for(int i=0; i < this->numEstados; i++){
			temp.clear();
			stringSt.clear();
			getline(sensor,temp);
			stringSt.str(temp);


			int j=0;
			cout << "Cidade " << i << " cidades adjacentes: ";

			while(stringSt >> valor_temp){
				this->cidades[i].cidadesAdjacentes[j++] = valor_temp;
				cout << " " << this->cidades[i].cidadesAdjacentes[j-1];
			}

			cout << endl;
			this->cidades[i].numeroCidadesAdjacentes = j;

			temp.clear();
			stringSt.clear();
			getline(sensor,temp);
			stringSt.str(temp);
			j=0;
			cout << "Distancias:";
			while(stringSt >> valor_temp){
				this->cidades[i].distanciaReal[j++] = valor_temp;
				cout << " " << this->cidades[i].distanciaReal[j-1];
			}

			cout << endl;
		}
	}

	heuristica.open("heuristica.txt");
	if(heuristica.fail()){
		cout << "Erro ao abrir o arquivo de heuristica!" << endl;
	}
	else{
		for(int i=0; i < this->numEstados; i++){
			getline(heuristica, temp);
			this->distanciaEstimada[i] = atoi(temp.c_str());
		}
	}

}

Agente::~Agente(){
}

list<int> Agente::melhorCaminho(int *caminho, int estado){
	list<int> lista;

	lista.push_front(estado);

	while(caminho[estado]!=-1){
		estado = caminho[estado];
		lista.push_front(estado);
	}
	return lista;
}

int Agente::A_estrela(){
	int *g_custo = new int[this->numEstados], g_tentativa,
		*f_custo = new int[this->numEstados],
		*antecessor = new int[this->numEstados], //Buscar de onde ele veio
		estadoAtual = this->estadoIni,
		openLnum = 0, indTemp=estadoAtual,
		*open_list = new int[this->numEstados],
		*closed_list = new int[this->numEstados];	//open list = nodes nao visitados, closed list = nodes visitados
	
	//Inicializando os vetores
	for(int i=0; i < this->numEstados; i++){
		g_custo[i] = 0;
		f_custo[i] = 0;
		open_list[i] = 0;
		closed_list[i] = 0;
	}

	antecessor[this->estadoIni] = -1;	//Inicialmente não existe antecessor

	g_custo[this->estadoIni] = 0;
	f_custo[this->estadoIni] = g_custo[this->estadoIni] + this->distanciaEstimada[this->estadoIni];//F = FC + FA

	open_list[this->estadoIni] = 1;
	openLnum++;

	//cout << "F do est inicial e " << f_custo[this->estadoIni] << endl;

	while(openLnum > 0 ){	//enquanto a open list nao estiver vazia a função continua
		int tmp=9999;
		for(int k =0; k < this->numEstados; k++){
			//pegar o elemento da openlist com menor funcao de custo
			if(open_list[k]==1){//Significa que o nó n foi visitado ainda
				cout << "Distancia: " << f_custo[k] << " (cid " << k << ")"<< endl;
				if(f_custo[k] < tmp){//Verificamos se o F_custo[k](FA+FC) é menor que o temporário. Se for
					cout << f_custo[k] << " menor que " << tmp << endl;
					tmp = f_custo[k]; //Armazena a melhor qualidade em temp
					indTemp= k; // Pega o índice da cidade com melhor qualidade
				}
			}
		}

		estadoAtual = indTemp;

		cout << endl << "Vai pro estado " << estadoAtual << endl;

		if(estadoAtual == this->estadoFin){
			cout << "Solucao encontrada:";
			list<int> tmp2 = melhorCaminho(antecessor,estadoAtual);
			list<int>::const_iterator it;
			for(it=tmp2.begin(); it!=tmp2.end(); ++it)
				cout << " " << *it;
			return 1;
		}			

		open_list[estadoAtual] = 0;		//remove da openlist
		openLnum--;
		closed_list[estadoAtual] = 1;	//add ao closedlist

		for(int i=0; i < this->cidades[estadoAtual].numeroCidadesAdjacentes; i++){
			if( closed_list[ this->cidades[estadoAtual].cidadesAdjacentes[i] ] == 1) //pertence a closed list
				continue;
			g_tentativa = g_custo[estadoAtual] + this->cidades[estadoAtual].distanciaReal[i];
			int temp = this->cidades[estadoAtual].cidadesAdjacentes[i]; //para melhor visualizar o codigo, node adjacente
			//cout << "Custo "<< g_tentativa << " cidade " << temp << endl;
			if((open_list[ temp ] == 0) || g_tentativa < g_custo[temp]){
				antecessor[temp] = estadoAtual;
				g_custo[temp] = g_tentativa;
				f_custo[temp] = g_custo[temp] + this->distanciaEstimada[temp];
				if( open_list[temp] == 0){
					cout << "Add " << temp <<" na openlist" << endl;
					open_list[temp] = 1;
					openLnum++;
				}
			}

		}



	}

	for(int i=0; i < this->cidades[estadoAtual].numeroCidadesAdjacentes; i++){

	}
	return 0;
}

int Agente::buscaProfundidade(Grafo *cidade , int inicio){	
	int *nos_visitados = new int[this->numEstados];

	int i , contador = 1;
	for(i = 0 ; i < this->numEstados ; i++){
		nos_visitados[i] = 0; //Inicializa todos os nós do grafo como não visitados
	}

	buscaProfundidadeAuxiliar(cidade , inicio, nos_visitados, contador); //Chama um método auxiliar para ir testando grafo a grafo
	

	return 0;

}

int Agente::buscaProfundidadeAuxiliar(Grafo *cidade , int inicio, int *nos_visitados,int contador){
	int i;
	nos_visitados[inicio] = contador; //Uma posição no vértice será visitada(contador = 1)	
	//Abaixo temos um laço que irá verificar as cidades adjacentes de cada vértice
	//Caso uma delas não tenha sido visitada então visitamos chamando a função de forma recursiva
	for(i = 0; i < cidade[inicio].numeroCidadesAdjacentes ; i++){
		if(!nos_visitados[cidade[inicio].cidadesAdjacentes[i]]){//Se for 0 significa que n foi visitado então será agora
			if(cidade[inicio].cidadesAdjacentes[i] == this->estadoFin){
				return 1;
			}
			cout << "No visitado :" << cidade[inicio].cidadesAdjacentes[i] << "\n";
			buscaProfundidadeAuxiliar(cidade , cidade[inicio].cidadesAdjacentes[i], nos_visitados , contador);
		}
	}

	return 0;
}

int Agente::buscaProfundidadeInicio(){	
	buscaProfundidade(this->cidades , this->estadoIni);	
	return 0;
}



int main (int argc, char *argv[]){
	Agente agente("teste.txt");
	agente.A_estrela();
	cout << "\nBUSCA EM PROFUNDIDADE \n";	
	agente.buscaProfundidadeInicio();

	return 0;

}

