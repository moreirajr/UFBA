#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstring>

using namespace std;
const int TONZ_CINZA = 256; //tons de cinza das imgs definido na especificacao do trabalho

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
Bitmap *Bmp_img_correct = NULL, *Bmp_img_output = NULL;

void BMP_Compare()
{
	float percent = 0;
	int similarity = 0;
	for(unsigned int i = 0; i < Bmp_img_correct->Size(); i++)
	{
		int indice = i * 3;
		//unsigned char pixelDif = abs( (Bmp_img_correct->DataValue(index) - Bmp_img_output->DataValue(index)) );
		
		similarity += abs( (Bmp_img_correct->DataValue(indice) - Bmp_img_output->DataValue(indice)) );

	}
	 
	percent = ( 1.0 - (similarity / (Bmp_img_correct->Size() * 255)) ) * 100;
	
	cout << endl <<  "SIMILARIDADE DAS IMAGENS: " << percent << endl;
}

int main(int argc, char* argv[])
{	// imagem correta (do professor) depois img output
	//	g++ -o comparer BMPcomparer.cpp
	//	./comparer picture_2_saida.bmp output_picture_2_entrada.bmp
	
	
	//comparar histograma:
	//	./comparer img_histogram.bmp output_picture_2_entrada.bmp

	
	if(argc < 2)
	{
		fprintf(stderr,"ERRO: numero de argumentos invalido! Informe o nome dos arquivo de entrada para realizar comparacao.");
		exit(1);
	}
	
	Bmp_img_correct = new Bitmap(argv[1]);
	Bmp_img_output = new Bitmap(argv[2]);
	
	BMP_Compare();

	return 0;
}
