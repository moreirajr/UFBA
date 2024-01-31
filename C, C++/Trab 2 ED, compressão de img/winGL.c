#include <stdio.h>
#include <stdlib.h>

#if defined (__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "winGL.h"
#include "glm.h"
#include "quadtree.h"

extern int 	iHeight, 
			iWidth;
		
						
/// ***********************************************************************
/// ** 
/// ***********************************************************************

unsigned char* copyGrayImage(unsigned char* img, int w, int h) {

	unsigned char* imgCpy = (unsigned char*) malloc ( sizeof(unsigned char) * w * h);

	int i,j;
	
	if (imgCpy != NULL)
		for (j = 0 ; j < h ; j++) 
			for (i = 0 ; i < w ; i++)
				imgCpy[j*w+i] = (	0.3*img[j*w*3+i*3+RED] +
									0.59*img[j*w*3+i*3+GREEN] +
									0.11*img[j*w*3+i*3+BLUE]);


	return imgCpy;
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

unsigned char* leImagem(char* filename) {
	
	unsigned char* 	image 		= NULL;
	unsigned char* 	imageGray 	= NULL;
	
    image = glmReadPPM(filename, &iWidth, &iHeight);
    
    if (image == NULL) {
    	printf("Erro carregando imagem %s\n", filename);
    	exit(0);
    	}
    
	printf("Imagem => %s\n", filename);
	printf("Resolucao => %d x %d\n", iHeight, iWidth);

    imageGray = copyGrayImage(image, iWidth, iHeight);
    
	if (imageGray == NULL) {
    	printf("Erro convercao imagem %s em tons de cinza\n", filename);
    	exit(0);
    	}

    free(image);
    	
    return imageGray;	
}
						
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaQuadrante(tPonto p0, tPonto p1, unsigned char c) {
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glColor3ub(c,c,c);
    glBegin(GL_QUADS);
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p0.y);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p0.x, p1.y);
    glEnd(); 

}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initOpenGL (void) {
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glShadeModel(GL_FLAT);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, w, 0.0, h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void criaJanela(int argc, char** argv) {
	
    glutInit(&argc, argv);

#if defined (__APPLE__) || defined(MACOSX)
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);	//nao funciona (windows)
#else
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
#endif

    glutInitWindowSize (iWidth, iHeight);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Quadtree - Estrutura de Dados");
}


/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initEventos() {
	
    glutDisplayFunc(desenho);
    glutKeyboardFunc(teclado);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	
	glutMainLoop();

}


