#ifndef __winGL__
#define __winGL__ 1

#include "opGeometricas.h"

typedef enum 	{	WHITE,
					RED,
					GREEN,
					BLUE,
					MAGENTA,
					CIAN,
					YELLOW,
					GRAY
				} eCor;

void desenhaPonto(tPonto P0, eCor c);
void desenhaLinha(tPonto P0, tPonto P1, eCor c);
void desenhaTriangulo(tPonto P0, tPonto P1, tPonto P2, eCor c);

void display(void);

void initOpenGL (void);

void reshape (int w, int h);

void keyboard (unsigned char key, int x, int y);

void criaJanela(int argc, char** argv);

void initEventos();

#endif

