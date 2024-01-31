#ifndef __winGL__
#define __winGL__ 1

#include "EstruturasDeDados.h"

typedef enum 	{	WHITE,
					RED,
					GREEN,
					BLUE,
					MAGENTA,
					CIAN,
					YELLOW,
					GRAY
				} eCor;

void desenhaQuadrante(tPonto p0, tPonto p1, unsigned char c);
unsigned char* leImagem(char* filename);
unsigned char* copyGrayImage(unsigned char* img, int w, int h);

void display(void);

void initOpenGL (void);

void reshape (int w, int h);

void keyboard (unsigned char key, int x, int y);

void criaJanela(int argc, char** argv);

void initEventos();

#endif

