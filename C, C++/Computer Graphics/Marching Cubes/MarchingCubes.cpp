/*
 * Universidade Federal da Bahia
 * Evaldo Machado Moreira Junior
 * Marching Cubes Algorithm
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
#include <sstream>
#include <vector>
#include <cmath>



using namespace std;
// ************************************************************************************************* //
// DEFINICOES //
// ************************************************************************************************* //

typedef int LUT[256][16]; //Look Up Table

typedef struct _ponto
{
	float x;
	float y;
	float z;
}Ponto;

typedef struct _vertice
{
	Ponto ponto;
	float valor;
	bool operator == (const struct _vertice &ve) const
	{	return ve.ponto.x == this->ponto.x && ve.ponto.y == this->ponto.y && ve.ponto.z == this->ponto.z;	};
}Vertice;

//variaveis globais:
LUT lutMc;
int LENGTH; //tamanho da entrada, N
float isoValor;
float ***amostras, N_cubos;




string ve_print(Vertice v)
{
	stringstream s;
	s << "v " << v.ponto.x << " " << v.ponto.y << " " << v.ponto.z <<  endl;
	return s.str();
}

class Triangulo
{
	Vertice vertice[3];

	public:
	Triangulo(Vertice v1, Vertice v2, Vertice v3)
	{
		vertice[0] = v1;
		vertice[1] = v2;
		vertice[2] = v3;
	}
	~Triangulo(){}
	Vertice getVertice(int ind){ return vertice[ind]; }
	Vertice CriarVertice(float px, float py, float pz)
	{
		Vertice ve;
		ve.ponto.x = px;
		ve.ponto.y = py;
		ve.ponto.z = pz;
		ve.valor = -1;
		return ve;
	}
};

class Cubo
{
	Vertice vertice[8];
	int aresta[12];
	vector<Triangulo> triangulos;
	bool mapeado;

	public:
	Cubo(int px, int py, int pz)
	{
		//sentido baixo pra cima, anti-horario
		vertice[4] = CriarVertice(px, py, pz);
		vertice[5] = CriarVertice(px+1, py, pz);
		vertice[0] = CriarVertice(px, py+1, pz);
		vertice[1] = CriarVertice(px+1, py+1, pz);

		vertice[7] = CriarVertice(px, py, pz+1);
		vertice[6] = CriarVertice(px+1, py, pz+1);
		vertice[3] = CriarVertice(px, py+1, pz+1);
		vertice[2] = CriarVertice(px+1, py+1, pz+1);

		std::fill_n(aresta, 11, -1);
		mapeado = false;
	}
	~Cubo(){};
	int TriangulosSize(){ return triangulos.size(); }
	Vertice getVertice(int ind){ return vertice[ind]; }
	Triangulo getTriangulo(int ind){ return triangulos[ind]; }
	vector<Triangulo> getTriangulos()
	{
		return triangulos;
	}

	Vertice CriarVertice(int px, int py, int pz)
	{
		Vertice ve;
		ve.ponto.x = px;
		ve.ponto.y = -py;
		ve.ponto.z = -pz;
		ve.valor = amostras[px][py][pz];
		return ve;
	}

	void InicializarArestas(int lutConfig)
	{
		int aux = 0;
		int* config = lutMc[lutConfig];

		while(config[aux] != -1)
		{
			aresta[aux] = config[aux];
			aux++;
		}
	}

	void CriarTriangulos()
	{
		int aux=0;
		while(aresta[aux] != -1)
		{
			Vertice v1, v2, v3;

			v1 = InterpolarPontos(aresta[aux]);
			v2 = InterpolarPontos(aresta[aux+1]);
			v3 = InterpolarPontos(aresta[aux+2]);

			Triangulo tri(v1, v2, v3);
			triangulos.push_back(tri);
			aux+=3;
		}
	}

	Vertice InterpolarPontos(int aresta)
	{
		Vertice ve;
		switch(aresta)
		{
		case 0://v0 e v1
			ve = VerticeInterpolado(vertice[0], vertice[1]);
			break;
		case 1://v1 e v2
			ve = VerticeInterpolado(vertice[1], vertice[2]);
			break;
		case 2://v3 e v2
			ve = VerticeInterpolado(vertice[3], vertice[2]);
			break;
		case 3://v0 e v3
			ve = VerticeInterpolado(vertice[0], vertice[3]);
			break;
		case 4://v4 e v5
			ve = VerticeInterpolado(vertice[4], vertice[5]);
			break;
		case 5://v5 e v6
			ve = VerticeInterpolado(vertice[5], vertice[6]);
			break;
		case 6://v7 e v6
			ve = VerticeInterpolado(vertice[7], vertice[6]);
			break;
		case 7://v4 e v7
			ve = VerticeInterpolado(vertice[4], vertice[7]);
			break;
		case 8://v0 e v4
			ve = VerticeInterpolado(vertice[0], vertice[4]);
			break;
		case 9://v1 e v5
			ve = VerticeInterpolado(vertice[1], vertice[5]);
			break;
		case 10://v2 e v6
			ve = VerticeInterpolado(vertice[2], vertice[6]);
			break;
		case 11://v3 e v7
			ve = VerticeInterpolado(vertice[3], vertice[7]);
			break;
		default:
			cout << "ERRO NO INDICE DO VERTICE";
			break;
		}

		return ve;
	}

	Vertice VerticeInterpolado(Vertice v1, Vertice v2)
	{
		float p1, p2, div;
		Vertice ve;

		div = abs(v2.valor - v1.valor);
		if(div == 0)
			p1 = p2 = 0;
		else
		{
			p1 = abs(isoValor - v2.valor) / div;
			p2 = abs(isoValor - v1.valor) / div;
		}

		//cout << endl << "p1: " << p1 << " p2: " << p2 << endl;

		ve.ponto.x = (v1.ponto.x * p1) + (v2.ponto.x * p2);
		ve.ponto.y = (v1.ponto.y * p1) + (v2.ponto.y * p2);
		ve.ponto.z = (v1.ponto.z * p1) + (v2.ponto.z * p2);
		ve.valor = -1;

		//cout << "Saida: " << ve_print(ve);

		return ve;
	}

	string Teste()
	{
		stringstream s;
		for(unsigned int i=0; i < triangulos.size(); i++)
		{
			s << "{" << ve_print(getTriangulo(i).getVertice(0))
			<< " " << ve_print(getTriangulo(i).getVertice(1))
			<< " " << ve_print(getTriangulo(i).getVertice(2)) << "}";
		}

		return s.str();
	}

	string TesteAresta()
	{
		stringstream s;
		int i=0;
		while(aresta[i] != -1){ s << " " << aresta[i++];}
		s << " ";
		return s.str();
	}

	string TesteCubo()
	{
		stringstream s;
		for(int i=0; i < 8; i++)
			s << endl << "Vertice " << i << ": " << ve_print(vertice[i]) << " - valor: " << vertice[i].valor;
		return s.str();
	}
};

class FileReader
{
	ifstream arquivo;

	public:
	FileReader(const char *filename);
	~FileReader();
};

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

FileReader::~FileReader(){}
FileReader::FileReader(const char *filename): arquivo(filename)
{
	string strTemp;
	int intTemp;
	istringstream iStringStream;

	if(arquivo.fail())
		cout << "Falha ao abrir o arquivo de entradas!" << endl;
	else
	{
		/* ler as configuracoes de entrada */
		for(int i=0; i < 256; i++)
		{
			strTemp.clear();
			iStringStream.clear();
			getline(arquivo, strTemp);

			iStringStream.str(strTemp);	//split

			int n=0;
			while(iStringStream >> intTemp)
			{
				lutMc[i][n] = intTemp;
				n++;
			}
		}

		strTemp.clear();
		iStringStream.clear();
		getline(arquivo, strTemp);

		iStringStream.str(strTemp);
		iStringStream >> LENGTH;

		/* (N-1)x(N-1)x(N-1) cubos, N=LENGTH */
		N_cubos = (int) pow((double) LENGTH-1, 3.0);

		/* inicializar vetor de amostras */
		amostras = new float**[LENGTH];
		for(int i=0; i<LENGTH; i++)
		{
			amostras[i]= new float*[LENGTH];
		    for(int k=0; k<LENGTH; k++)
		    	amostras[i][k]= new float[LENGTH];
		}

		/* ler segunda parte da entrada, amostras*/
		for(int z=0; z < LENGTH; z++)
			for(int x=0; x < LENGTH; x++)
			{
				strTemp.clear();
				iStringStream.clear();
				getline(arquivo, strTemp);

				iStringStream.str(strTemp);	//split

				int y=0;
				while(iStringStream >> intTemp)
				{
					amostras[x][y][z] = intTemp;
					y++;
				}
			}


		strTemp.clear();
		iStringStream.clear();
		getline(arquivo, strTemp);

		iStringStream.str(strTemp);
		iStringStream >> isoValor;
	}
}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

int LUT_config(Cubo c)
{
	double config=0;

	for(int i=0; i < 8; i++)
	{
		if(c.getVertice(i).valor == 1)
			config += pow(2.0, (double)i);
	}

	return (int)config;
}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

void InicializarCubos(vector<Cubo> &cubos)
{
	/* (N-1)x(N-1)x(N-1) cubos, N=LENGTH */
	for(int z=0; z < LENGTH -1; z++)
		for(int y=0; y < LENGTH -1; y++)
			for(int x=0; x < LENGTH -1; x++)
			{
				Cubo c(x,y,z);
				cubos.push_back(c);
			}

	for(int i=0; i< N_cubos; i++)
	{
		int config = LUT_config(cubos[i]);
		cubos[i].InicializarArestas(config);
	}
}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

int VerificarVerticeDuplicado(vector<Vertice> vec_ve, Vertice ve)
{
	for(unsigned int i=0; i < vec_ve.size(); i++)
		if(vec_ve[i] == ve)
			return i;

	return -1;
}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

void GerarSaida(vector<Cubo> cubos)
{
	char filename[] = "malha.obj";
	fstream fsArquivo;
	vector<Vertice> malha;
	vector<Triangulo> vec_tri;

	//N_cubos
	for(int i=0; i < N_cubos; i++)
	{
		for(unsigned int j= 0; j < cubos[i].getTriangulos().size(); j++)
			vec_tri.push_back(cubos[i].getTriangulo(j));
	}
	//cout << "Triangulos na malha: " << vec_tri.size();


	fsArquivo.open(filename,  fstream::in | fstream::out | fstream::trunc);


	int count_tri=1;
	stringstream s;
	for(unsigned int i=0; i < vec_tri.size(); i++)
	{
		int face[3], f=0, indice;

		for(int j=0; j < 3; j++)
		{
			indice = VerificarVerticeDuplicado( malha, vec_tri[i].getVertice(j) );

			//cout << "[indice: " << indice << "]  f: " << f << endl;
			/* vertice nao esta na malha */
			if(indice == -1)
			{
				malha.push_back(vec_tri[i].getVertice(j));
				face[f++] = count_tri++;
				fsArquivo << ve_print(vec_tri[i].getVertice(j));
			}
			else
			{
				face[f++] = indice+1;
			}
		}

		s << "f " << face[0] << " " << face[1] << " " << face[2] << endl;
	}

	fsArquivo << s.str();
	fsArquivo.close();

}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

void MarchingCubes(vector<Cubo> cubos)
{
	for(int i=0; i < N_cubos; i++)
	{
		//cout << "Cubo " << i << ": ";
		//cout << "Arestas do cubo que formam triangulo: [" << cubos[i].TesteAresta() << "]" << endl;

		cubos[i].CriarTriangulos();

		//cout << endl <<"Triangulos do Cubo " << i << ": " << endl << cubos[i].Teste() << endl;
		//cout << endl << cubos[i].TesteCubo() << endl;
	}

	GerarSaida(cubos);
}

// ************************************************************************************************* //
// ************************************************************************************************* //
// ************************************************************************************************* //

int main (int argc, char *argv[])
{
	//executar no terminal: (./executavel nomeArquivo.txt)
	//CG_MarchingCubes.exe Entrada_MC.txt
	//testar saida em: http://3dviewer.net

	if(argc < 1)
	{
		fprintf(stderr,"ERRO: numero de argumentos invalido! Informe o nome do arquivo de entrada.");
		exit(1);
	}

	FileReader arquivoEntrada(argv[1]);

	vector<Cubo> CubosMc;
	InicializarCubos(CubosMc);

	MarchingCubes(CubosMc);

	return 0;
}
