#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined (__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "EstruturasDeDados.h"
#include "Triangulacao.h"
#include "winGL.h"
		
int		wLargura 	= 500,
		wAltura 	= 500;
	
tPonto 	triangulo[3];
int		numPtos=-1;
		
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaPoligonal() {
}
		
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaTriangulacao() {
}
		
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void geraTriangulacao() {
	
	printf("Rotina a ser implementada\n");
	
/*  Algoritmo Triangulacao

		Passo 1: Classifica todos os vertices se sao arelhas; */
		
	initVertices();
		
/*		Passo 2: Enquanto for possivel formar triangulos na poligal
					Chama rotina Triangulação;
*/     	

	Triangular();
}


/// ***********************************************************************
/// ** 
/// ***********************************************************************

void teclado(unsigned char key, int x, int y) {
	switch (key) {
		case 27		: 	exit(0);
						break;				
		case 't'	: 	
		case 'T'	: 	geraTriangulacao();
						break;				
		}
	glutPostRedisplay();
}
		
/// ***********************************************************************
/// **
/// ***********************************************************************

void mouse(int button, int button_state, int x, int y ) {

	if 	(button_state == GLUT_DOWN ) {
		switch (button) {
			
			case GLUT_LEFT_BUTTON	: 	numPtos++;
										triangulo[numPtos%3].x = x;
										triangulo[numPtos%3].y = wAltura - y;
										break;
	
			case GLUT_RIGHT_BUTTON	:	printf("Fazer alguma coisa com o botao direito?\n");
										break;
			}
		glutPostRedisplay();
		}
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenho(void) {

    glClear (GL_COLOR_BUFFER_BIT); 
    
    glColor3f (1.0, 1.0, 1.0);
    
    switch (numPtos%3) {
    	case 0	:	desenhaPonto(triangulo[0], BLUE);
    				break;
    	case 1	:	desenhaLinha(triangulo[0], triangulo[1], YELLOW);
    				break;
    	case 2	:	desenhaTriangulo(triangulo[0], triangulo[1], triangulo[2], GREEN);
    				break;
    	}
    	
    desenhaPoligonal();
    
    desenhaTriangulacao();
    	
    glutSwapBuffers ();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

int main(int argc, char** argv) {
	
	criaJanela(argc, argv);

    initOpenGL();
    
    initEventos();
    
    return 0;   
}
