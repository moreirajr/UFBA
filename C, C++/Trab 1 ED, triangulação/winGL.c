#include <stdio.h>
#include <stdlib.h>

#if defined (__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "trabalho.h"
#include "winGL.h"
#include "Triangulacao.h"
		
extern GLint	wLargura,
				wAltura;
				
float Cores[8][3] = { 	{1.0, 1.0, 1.0},
						{1.0, 0.0, 0.0},
						{0.0, 1.0, 0.0},
						{0.0, 0.0, 1.0},
						{1.0, 0.0, 1.0},
						{0.0, 1.0, 1.0},
						{1.0, 1.0, 0.0},
						{0.5, 0.5, 0.5} };
						
						
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaPonto(tPonto P, eCor c) {

	glColor3fv(Cores[c]);

    glBegin(GL_POINTS);
		glVertex2f(P.x, P.y);
    glEnd(); 
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaLinha(tPonto P0, tPonto P1, eCor c) {

	glColor3fv(Cores[c]);

    glBegin(GL_LINES);
		glVertex2f(P0.x, P0.y);
		glVertex2f(P1.x, P1.y);
    glEnd(); 
}
	
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaTriangulo(tPonto P0, tPonto P1, tPonto P2, eCor c) {
	
	glColor3fv(Cores[c]);

    glBegin(GL_TRIANGLES);
		glVertex2f(P0.x, P0.y);
		glVertex2f(P1.x, P1.y);
		glVertex2f(P2.x, P2.y);
    glEnd(); 
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initOpenGL (void) {
	glClearColor (0.0, 0.0, 0.0, 0.0); 
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
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (wAltura, wLargura);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Triangulacao - Estrutura de Dados");
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


