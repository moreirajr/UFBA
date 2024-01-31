/*
 * Aluno: Evaldo Machado Moreira Junior
 * Universidade Federal da Bahia - UFBA
 * 25/11/2017
 *
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstring>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif


#define SIGMA_S 3
#define SIGMA_R 8
#define WINDOW_SIZE 9

using namespace std;
const int TONZ_CINZA = 256; //tons de cinza das imgs definido na especificacao do trabalho

/* ********************************************************** */
/* ****************** Definicoes **************************** */
/* ********************************************************** */

typedef struct _pr
{
	int count;
	float prob;
	float tomCinzaNormalizado;
	int new_index;
} Pr;

typedef struct _h
{
	Pr pixels_pr[TONZ_CINZA];
	Pr pixels_sr[TONZ_CINZA];
} Histogram;

class Bitmap
{
	static const int HEADER_SIZE = 54;
	static const int CHANNELS = 3;		// RGB channels
	unsigned char *header;				// Each BMP file begins by a 54-bytes header
	unsigned int dataPos;				// Position in the file where the actual data begins
	unsigned int width, height;			// Image width and height
	unsigned int imageSize;				// width*height*channels
	unsigned char* data;				// Actual RGB data
	string bmp_filename;
	ifstream img_file_bmp;
	
public:
	Bitmap();
	Bitmap(const char *filename);
	~Bitmap();
	int Channels() { return this->CHANNELS;}
	unsigned int  Width(){	return this->width;	}
	unsigned int Height(){	return this->height;}
	unsigned int Size() { return this->width * this->height; }
	unsigned char *Data(){	return this->data;	}
	unsigned char DataValue(int i){	return this->data[i];	}
	void SetPixel(int i, unsigned char value){ data[i] = value; }
	void SaveToFile();
};

Bitmap::Bitmap(){}
Bitmap::~Bitmap(){}
void Bitmap::SaveToFile()
{
	const char *file =  ("output_" + bmp_filename).c_str();
	ofstream bmp_file(file, ios::out | ios::binary);

	if (bmp_file.is_open()) {
	
		bmp_file.write( (char*) header, HEADER_SIZE);
		bmp_file.write( (char*) data, imageSize);
	}
	
	bmp_file.close();
}
Bitmap::Bitmap(const char *filename): img_file_bmp(filename, ios::binary)
{
	if(img_file_bmp.fail())
	{
		cout << "Falha ao abrir o arquivo!";
		exit(1);
	}
	
	bmp_filename = filename;

	unsigned char aux;
	char *reader_aux = new char[HEADER_SIZE];
	header = new unsigned char[HEADER_SIZE];
	
	img_file_bmp.read(reader_aux, HEADER_SIZE);
	for(int i=0; i < HEADER_SIZE; i++)
	{
		//img_file_bmp >> hex >> aux; //nao funciona
		//header[i] = aux;
		header[i] = reader_aux[i];
	}
	
	if( (img_file_bmp.gcount() < HEADER_SIZE) || (header[0]!='B' || header[1]!='M') )
	{
		cout << "A imagem nao esta no formato BMP!";
		exit(1);
	}
	
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
								// 3 channels : one byte for each Red, Green and Blue component
	if (imageSize==0)    imageSize = width * height * CHANNELS;
								// The BMP header is done that way
	if (dataPos==0)     dataPos = HEADER_SIZE;

	// Create a buffer
	data = new unsigned char[imageSize];
	reader_aux = new char[imageSize];
	
	// Read the actual data from the file into the buffer
	img_file_bmp.read(reader_aux, imageSize);
	for(unsigned int i=0; i < imageSize; i++)
	{
		//img_file_bmp >> hex >> aux; //nao funciona
		//data[i] = aux;
		data[i] = reader_aux[i];
	}
	
	for(unsigned int i = 0; i < width * height ; i++)
	{
		int index = i * CHANNELS;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];
		data[index] = R;
		data[index + 2] = B;
	}

	img_file_bmp.close();
}

/* ********************************************************** */
/* ******************** VARIAVEIS GLOBAIS******************** */
/* ********************************************************** */
Bitmap *Bmp_img_original = NULL, *Bmp_img_output = NULL;
Histogram histogram;
/* ********************************************************** */

int NearestValue(Pr *indices_normalizados, float value)
{
	int indice = 0, i = 0;
			
	while(indices_normalizados[i].tomCinzaNormalizado < value) i++;
		
	if(i == 0 || i == TONZ_CINZA-1) return i;

	if( indices_normalizados[i+1].tomCinzaNormalizado - value < value - indices_normalizados[i].tomCinzaNormalizado)
		indice = i+1;
	else
		indice = i;

	return indice;
}

/* ********************************************************** */
void PrintHistogram()
{
	float pt_r = 0, pt_s = 0;
	for(int i=0; i < TONZ_CINZA; i++)
	{
		pt_r += histogram.pixels_sr[i].prob;
		pt_s += histogram.pixels_sr[i].prob;
		cout << "S" << i << "(" << histogram.pixels_pr[i].tomCinzaNormalizado << ")" <<	" [Count: " << histogram.pixels_pr[i].count << "]"
		<< " - pr: " << histogram.pixels_pr[i].prob
		<< " sr: " << histogram.pixels_sr[i].prob << " (" << i << " => " << histogram.pixels_pr[i].new_index << ")" << endl;
	}
	cout << "Total:\t" << Bmp_img_original->Size() << "\t" << pt_r <<  "\t" << pt_s << endl;
}

/* ********************************************************** */

void HistogramEqualization()
{
	memset(&histogram.pixels_pr, 0, sizeof(Pr) * TONZ_CINZA);
	memset(&histogram.pixels_sr, 0, sizeof(Pr) * TONZ_CINZA);
	
	for(unsigned int i=0; i < Bmp_img_original->Size(); i++)
	{
		int indice = i * Bmp_img_original->Channels();
		unsigned char pixelValue = Bmp_img_original->DataValue(indice);
		histogram.pixels_pr [ pixelValue ].count++; //+= 3;
	}

	//Calcular probabilidade e probabilidade acumulada
	float prob_acumulada = 0;
	for(int i=0; i < TONZ_CINZA; i++)
	{
		histogram.pixels_pr[i].prob = (float) histogram.pixels_pr[i].count / ( Bmp_img_original->Size() ); // * 3
		histogram.pixels_pr[i].tomCinzaNormalizado = (float) i / (TONZ_CINZA-1);
		
		prob_acumulada += histogram.pixels_pr[i].prob;
		histogram.pixels_sr[i].prob = prob_acumulada;
	}
	
	for(int i=0; i < TONZ_CINZA; i++)
	{
		int novoIndice = NearestValue(histogram.pixels_pr, histogram.pixels_sr[i].prob);
		histogram.pixels_sr[i].new_index = novoIndice;
	}
	
	//PrintHistogram();
	
	for(unsigned int i=0; i < Bmp_img_output->Size(); i++)
	{
		int indice = i * Bmp_img_output->Channels();
		unsigned char pixelValue = Bmp_img_output->DataValue(indice);
			
		for(int j=0; j < Bmp_img_output->Channels(); j++)
			Bmp_img_output->SetPixel(indice + j, (unsigned char) histogram.pixels_sr[ pixelValue ].new_index);
	}
}

/* ********************************************************** */

float Gaussian_Function(int x, int sigma)
{
	float sig = pow(sigma, 2);
	float g_result = (1.0 / ( 2.0 * M_PI * sig)) * exp(-pow(x,2) / (2.0 * sig));

	return g_result;
}

/* ********************************************************** */

bool OutOfImg(int pixel, int size)
{
	return (pixel < 0 || pixel > size);
}

/* ********************************************************** */

float BilateralFormula(int index, int windowPixel_index, unsigned char pixelValue, unsigned char windowPixelValue)
{
	return Gaussian_Function( abs(index - windowPixel_index), SIGMA_S) * Gaussian_Function( pixelValue - windowPixelValue, SIGMA_R);
}

/* ********************************************************** */

void Teste(int index_center, int index)
{
	float output_wp, gs, gr;
	gs = Gaussian_Function( abs(index_center - index), SIGMA_S );
	gr = Gaussian_Function( Bmp_img_output->DataValue(index_center) - Bmp_img_output->DataValue(index), SIGMA_R );
	output_wp = gs * gr;
	cout << "P: " << index_center << "\tQ: " << index <<  "\tIp: " << (int) Bmp_img_output->DataValue(index_center) << "\tIq: " << (int)Bmp_img_output->DataValue(index) << endl;
	cout << "Gs: " <<  gs << "\tGr: " << gr << endl;
	cout << "Wp: " << output_wp << "\tWp_Iq: " << output_wp * Bmp_img_output->DataValue(index) << endl;
}

/* ********************************************************** */

float SumCollumnPixels(int index, float *out_Wp, int index_center)
{	
	float Wp_collumn = 0;
	int img_collumn, left_range, right_range, pixels_left, pixels_right;
	
	img_collumn = floor(index / Bmp_img_output->Width());
	
	left_range = img_collumn * Bmp_img_output->Width();
	right_range = Bmp_img_output->Width() + (img_collumn * Bmp_img_output->Width());
	
	pixels_left = index - left_range;
	pixels_right = index - right_range;
	
	unsigned char pixel = Bmp_img_output->DataValue(index);
	float Wp_aux = BilateralFormula(index_center, index, Bmp_img_output->DataValue(index_center), Bmp_img_output->DataValue(index));
	(*out_Wp) += Wp_aux;
	Wp_collumn += Wp_aux * (float) Bmp_img_output->DataValue(index);

	//TESTE
	//Teste(index_center, index);
	
	int aux = pixels_left, index_aux;
	while(aux > 0)
	{
		index_aux = index - (aux*3);
		pixel = Bmp_img_output->DataValue(index_aux);
		Wp_aux = BilateralFormula(index_center, index_aux, Bmp_img_output->DataValue(index_center), Bmp_img_output->DataValue(index_aux));
		(*out_Wp) += Wp_aux;
		Wp_collumn += Wp_aux * (float) Bmp_img_output->DataValue(index_aux);
		
		//TESTE
		//Teste(index_center, index_aux);
		
		aux -= 3;
	}
	
	aux = pixels_right;
	while(aux > 0)
	{
		index_aux = index + (aux*3);
		pixel = Bmp_img_output->DataValue(index_aux);
		Wp_aux = BilateralFormula(index_center, index_aux, Bmp_img_output->DataValue(index_center), Bmp_img_output->DataValue(index_aux));
		(*out_Wp) += Wp_aux;
		Wp_collumn += Wp_aux * (float) Bmp_img_output->DataValue(index_aux);
		
		//TESTE
		//Teste(index_center, index_aux);
		
		aux -= 3;
	}
	
	return Wp_collumn;
}

/* ********************************************************** */

float Wp_Normalization(int index_p, float *out_Wp)
{
	//9x9 window:	-12	-9	-6	-3	0	+3	+6	+9	+12
	//12 => (windowsize-1/2) * bmp.channels
	float Wp = 0;
	int pixel_horizontal, up_collumns, down_collumns, aux = 0;
	
	up_collumns = down_collumns = (WINDOW_SIZE-1)/2;
	
	//Coluna do meio
	Wp += SumCollumnPixels(index_p, out_Wp, index_p);

	//Pixels acima
	pixel_horizontal = index_p - (Bmp_img_output->Width() * 3);
	while(up_collumns > 0)
	{
		if(OutOfImg(pixel_horizontal, Bmp_img_output->Size() * 3))
			break;
			
		Wp += SumCollumnPixels(pixel_horizontal, out_Wp, index_p);
		pixel_horizontal -= (Bmp_img_output->Width() * 3);
		up_collumns--;
	}
	
	//Pixels abaixo
	pixel_horizontal = index_p + (Bmp_img_output->Width() * 3);
	while(down_collumns > 0)
	{
		if(OutOfImg(pixel_horizontal, Bmp_img_output->Size() * 3))
			break; 
		Wp += SumCollumnPixels(pixel_horizontal, out_Wp, index_p);
		pixel_horizontal += (Bmp_img_output->Width() * 3);
		down_collumns--;
	}
		
	return Wp;
}

/* ********************************************************** */

void BilateralFilter()
{
	for(unsigned int i=0; i < Bmp_img_output->Size(); i++)
	{
		int indice = i * Bmp_img_output->Channels();
		float Wp_Iq = 0, Wp = 0;
		
		Wp_Iq = Wp_Normalization(indice, &Wp);
		unsigned char BF_Ip =  (unsigned char) ((1.0/Wp) * Wp_Iq );
		
		for(int j=0; j < Bmp_img_output->Channels(); j++)
			Bmp_img_output->SetPixel(indice + j, BF_Ip);
	}
}

/* ********************************************************** */
/* ****************** OPENGL ******************************** */
/* ********************************************************** */

void init(void)
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glClearColor (0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glRasterPos2i (0, 0);
	glDrawPixels(Bmp_img_original->Width(), Bmp_img_original->Height(), GL_RGB, GL_UNSIGNED_BYTE, Bmp_img_original->Data());
	
	glRasterPos2i (Bmp_img_original->Width(), 0);
	glDrawPixels(Bmp_img_output->Width(), Bmp_img_output->Height(), GL_RGB, GL_UNSIGNED_BYTE, Bmp_img_output->Data());

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:	exit(0);
	}
}

/* ********************************************************** */
/* ********************************************************** */
/* ********************************************************** */

int main(int argc, char* argv[])
{
	//	g++ -o trab2 RestauracaoFoto.cpp -lGL -lGLU -lglut
	//	./trab2 picture_2_entrada.bmp
	if(argc < 1)
	{
		fprintf(stderr,"ERRO: numero de argumentos invalido! Informe o nome do arquivo de entrada.");
		exit(1);
	}
	
	Bmp_img_original = new Bitmap(argv[1]);
	Bmp_img_output = new Bitmap(argv[1]);
	
	//Aplicar filtros:
	cout << "Aplicando Equalizacao de Histograma...";
	HistogramEqualization();
	cout << "\t[Done]" << endl;
	cout << "Aplicando Filtro Bilateral...";
	BilateralFilter();
	cout << "\t[Done]" << endl;
	//
	Bmp_img_output->SaveToFile();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(Bmp_img_original->Width()*2, Bmp_img_original->Height());
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
