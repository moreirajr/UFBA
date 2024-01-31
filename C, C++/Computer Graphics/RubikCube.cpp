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


#define NUM_VERTICES 8
#define NUM_FACES 6
#define NUM_CUBOS_POR_LVL 2 //2x2x2
#define ANGULO 90


using namespace std;


typedef struct _color{
	float R, G, B, A;
} Color;

int angulo_x_lvl1, angulo_y_lvl1,
	angulo_x_lvl2, angulo_y_lvl2;


Color colors[6];

void DesenharCubo(Color color)
{
	//Cubo localizado na Origem (0,0,0), cada linha um vertice (xyz)
	static GLint vertices[ NUM_VERTICES * 3] = {
		1,	-1,	1,
		1,	-1,	-1,
		1,	1,	-1,
		1,	1,	1,
		-1,	-1,	1,
		-1, -1,	-1,
		-1,	1,	-1,
		-1,	1,	1
	};

	//indices dos vertices acima (linhas)
	static GLubyte faces[ NUM_FACES * 4] = {
		0, 1, 2, 3,
		4, 5, 1, 0,
		7, 6, 5, 4,
		3, 2, 6, 7,
		4, 0, 3, 7,
		5, 1, 2, 6
	};

	// static GLfloat cores[8*4] = {
	// 	1.0, 0.0, 0.0, 1.0,	//frente abaixo dir
	// 	0.0, 0.0, 1.0, 1.0,	//abaixo
	// 	0.0, 1.0, 0.0, 1.0,	//acima top dir
	// 	1.0, 0.0, 0.0, 1.0,	//frente acima dir
	// 	1.0, 0.0, 0.0, 1.0,	//frente baixo esq
	// 	0.0, 0.0, 1.0, 1.0, //acima 2x
	// 	0.0, 1.0, 0.0, 1.0,	//acima, top esq
	// 	1.0, 0.0, 0.0, 1.0	//frente acima esq
	// };


	static GLfloat cores[ NUM_VERTICES * 3] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
  
	glVertexPointer(3, GL_INT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, cores);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces); // 24 = sizeof(faces)/sizeof(GLubyte)

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

/* *************************************************************************************************** */

void DrawCube(int x, int y, int z)
{
	//Cubo localizado na Origem (0,0,0), cada linha um vertice (xyz)
	static GLint vertices[ NUM_VERTICES * 3] = {
		x,	-y,	z,
		x,	-y,	-z,
		x,	y,	-z,
		x,	y,	z,
		-x,	-y,	z,
		-x, -y,	-z,
		-x,	y,	-z,
		-x,	y,	z
	};

	//indices dos vertices acima (linhas)
	static GLubyte faces[ NUM_FACES * 4] = {
		0, 1, 2, 3,
		4, 5, 1, 0,
		7, 6, 5, 4,
		3, 2, 6, 7,
		4, 0, 3, 7,
		5, 1, 2, 6
	};

	static GLfloat cores[ NUM_VERTICES * 3] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 1.0,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
  
	glVertexPointer(3, GL_INT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, cores);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces); // 24 = sizeof(faces)/sizeof(GLubyte)

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

/* *************************************************************************************************** */

void InicializarCores()
{
	//RED
	colors[0].R = 1.0;
	colors[0].G = 0.0;
	colors[0].B = 0.0;

	//GREEN
	colors[1].R = 0.0;
	colors[1].G = 1.0;
	colors[1].B = 0.0;

	//BLUE
	colors[2].R = 0.0;
	colors[2].G = 0.0;
	colors[2].B = 1.0;

	//WHITE
	colors[3].R = 1.0;
	colors[3].G = 1.0;
	colors[3].B = 1.0;
}

void Inicializa()
{
	angulo_x_lvl1 = angulo_x_lvl2 = 0;
	angulo_y_lvl1 = angulo_y_lvl2 = 0;

	InicializarCores();

	//Janela de visualizacao
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//Sistema de coordenadas
	glOrtho(-10.0, 10.0, -10.0, 10.0, -6.0, 6.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Cor de fundo da janela
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

//Funcao callback chamada para fazer o desenho
void Desenha()
{
	// Limpa a janela de visualizacao com a cor de fundo especificada
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int c = 0;
	
	double tx, ty, tz;
	tx, ty, tz = 0.0;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );

	bool teste = true;

	if(teste)
	for(int x = 0; x < NUM_CUBOS_POR_LVL; x++)
	{	
		for(int y = 0; y < NUM_CUBOS_POR_LVL; y++)
		{
			for(int z = 0; z < NUM_CUBOS_POR_LVL; z++)
			{
				glPushMatrix();	
				glTranslatef(tx, ty, tz);
				x == 0? glRotatef(angulo_x_lvl1, 1.0, 0.0, 0.0) : glRotatef(angulo_x_lvl2, 1.0, 0.0, 0.0);
				y == 0? glRotatef(angulo_y_lvl1, 0.0, 1.0, 0.0) : glRotatef(angulo_y_lvl2, 0.0, 1.0, 0.0);
				
				DesenharCubo(colors[c++]);
				
				glPopMatrix();
				
				tz += 2.0;
			}

			tz = 0.0;
			ty += 2.0;
			c= 0;
		}
			

		ty = tz = 0.0;
		tx += 2.0;

	}
	else
		DrawCube(1, 1, 1);
	
	// Executa os comandos OpenGL para renderizacao
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//X e C movem os cubos verticalmente
		case 'x':
		angulo_y_lvl1 = angulo_y_lvl2 = 0;
		angulo_x_lvl1 = (angulo_x_lvl1 + 3) % 360;
		glutPostRedisplay();
		break;

		case 'c':
		angulo_y_lvl1 = angulo_y_lvl2 = 0;
		angulo_x_lvl2 = (angulo_x_lvl2 + 3) % 360;
		glutPostRedisplay();
		break;

		//Y e U movem os cubos y
		case 'y':
		angulo_x_lvl1 = angulo_x_lvl2 = 0;
		angulo_y_lvl1 = (angulo_y_lvl1 + 3) % 360;
		glutPostRedisplay();
		break;

		case 'u':
		angulo_x_lvl1 = angulo_x_lvl2 = 0;
		angulo_y_lvl2 = (angulo_y_lvl2 + 3) % 360;
		glutPostRedisplay();
		break;

		case 27:	exit(0);
		break;
	}
}

// Programa principal
int main(int argc, char** argv)
{
	//Compilar:
	//  g++ -o RubikCube RubikCube.cpp -lGL -lGLU -lglut

	// Inicia GLUT e processa argumentos passados por linha de comandos
	glutInit(&argc, argv);

	// Avisa a GLUT que tipo de modo de exibicaoo deve ser usado quando a janela e criada
	//usar o z-bufer => add GLUT_DEPTH
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// Cria uma janela GLUT que permite a execucao de comandos OpenGL
	glutInitWindowSize(1000,700); 
	glutCreateWindow("Trab3 - Pocket Cube");

	// Define a funcao respons�vel por redesenhar a janela OpenGL sempre que necessario
	glutDisplayFunc(Desenha);
	glutIdleFunc(Desenha);
	glutKeyboardFunc(keyboard);
	
	// Inicializacoes de OpenGL executadas antes da exibicao do desenho
	Inicializa();

	// Inicia o processamento de eventos de GLUT. O controle do programa
	// passa a GLUT, que inicia o gerenciamento dos eventos
	glutMainLoop();

	return 0;
}