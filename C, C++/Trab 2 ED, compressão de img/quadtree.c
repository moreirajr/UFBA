#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "EstruturasDeDados.h"
#include "winGL.h"
#include "quadtree.h"

unsigned char* 	imageGray = NULL;	// image file
int 			iHeight, 
	iWidth;

tNoArvore* 		raiz = NULL;

int desenha = 0;
float percent = 5; // porcentagem global
int nivel_global=0,nivel_maximo=0; //nivel global
// ***********************************************
// ******                                   ******
// ***********************************************

void em_ordem ( tNoArvore *pt) { 
	tPonto p1;

	if (pt) { 

		if(pt->q->nivel <= nivel_global){
			p1.x = pt->q->pBase.x + pt->q->w;
			p1.y = pt->q->pBase.y + pt->q->h;
			desenhaQuadrante(pt->q->pBase, p1,pt->q->cor);
		} 
		em_ordem (pt->filho[0]); 
		em_ordem (pt->filho[1]);
		em_ordem (pt->filho[2]);
		em_ordem (pt->filho[3]);

	} 
}

void desenhaQuadtree( tNoArvore *pt) {

	tPonto p0, p1;


	em_ordem (pt); 


}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void cor_uniforme(tQuadrante * quadrante) {

	int i,j,soma=0,cont=0,pixel,chave;
	float media,limite_inferior,limite_superior,taxa_erro;


	for (j = quadrante->pBase.y ; j < quadrante->pBase.y + quadrante->h ; j++) 
		for (i = quadrante->pBase.x ; i < quadrante->pBase.x + quadrante->w; i++) {

			soma = soma + imageGray[i+(iWidth*j)];
			cont++;


		}

		media = soma/cont;
		taxa_erro = (percent/100)*media;

		limite_superior = media + taxa_erro;
		limite_inferior = media - taxa_erro;
		chave = 0;
		for (j = quadrante->pBase.y ; j < quadrante->pBase.y + quadrante->h ; j++) 
			for (i = quadrante->pBase.x ; i < quadrante->pBase.x + quadrante->w; i++) {
				pixel = imageGray[j*iWidth+i];
				if(pixel<limite_inferior || pixel > limite_superior){
					quadrante->eFolha = 0;
					chave = 1;
				}
			}
			if(chave == 0){
				quadrante->eFolha = 1;
			}
			quadrante->cor =  floorf(media);


}

tNoArvore * montaQuadtree(int x, int y, int w, int h,int nivel) {

	tQuadrante *quadrante,*quadrante1,*quadrante2,*quadrante3,*quadrante4,*quadranteTemp;
	tNoArvore *filho1,*filho2,*filho3,*filho4,*filhoTemp;	
	int i,cor;



	if(raiz == NULL){
		raiz = (tNoArvore *) malloc(sizeof(tNoArvore));
		quadrante = (tQuadrante *) malloc(sizeof(tQuadrante));
		raiz->q = quadrante;
		quadrante->pBase.x = 0;
		quadrante->pBase.y = 0;
		quadrante->h = iHeight;
		quadrante->w = iWidth;
		quadrante->nivel = nivel;
		if(nivel_maximo < nivel){
			nivel_maximo = nivel;
			nivel_global = nivel;
		}
		cor_uniforme(quadrante);

		if(quadrante->eFolha == 1){
			for(i=0;i<4;i++){
				raiz->filho[i] = NULL;

			}
		}else{
			filho1 = montaQuadtree(quadrante->pBase.x, quadrante->pBase.y + h/2, w/2,  h/2,nivel+1);
			filho2 = montaQuadtree(quadrante->pBase.x + w/2, quadrante->pBase.y + h/2, w/2,  h/2,nivel+1);
			filho3 = montaQuadtree(quadrante->pBase.x + w/2, quadrante->pBase.y, w/2,  h/2,nivel+1);
			filho4 = montaQuadtree(quadrante->pBase.x, quadrante->pBase.y, w/2,  h/2,nivel+1);
			raiz->filho[0] = filho1;
			raiz->filho[1] = filho2;
			raiz->filho[2] = filho3;
			raiz->filho[3] = filho4;
		}
		return NULL;

	}else{
		filhoTemp = (tNoArvore *) malloc(sizeof(tNoArvore));	
		quadranteTemp = (tQuadrante *) malloc(sizeof(tQuadrante));
		filhoTemp->q = quadranteTemp;
		quadranteTemp->pBase.x = x;
		quadranteTemp->pBase.y = y;
		quadranteTemp->h = h;
		quadranteTemp->w = w;
		quadranteTemp->nivel = nivel;
		if(nivel_maximo < nivel){
			nivel_maximo = nivel;
			nivel_global = nivel;
		}
		cor_uniforme(quadranteTemp);
		if(quadranteTemp->eFolha == 1){
			for(i=0;i<4;i++){
				filhoTemp->filho[i] = NULL;

			}
		}else{
			filho1 = montaQuadtree(quadranteTemp->pBase.x, quadranteTemp->pBase.y + h/2, w/2,  h/2,nivel+1);
			filho2 = montaQuadtree(quadranteTemp->pBase.x + w/2, quadranteTemp->pBase.y + h/2, w/2,  h/2,nivel+1);
			filho3 = montaQuadtree(quadranteTemp->pBase.x + w/2, quadranteTemp->pBase.y, w/2,  h/2,nivel+1);
			filho4 = montaQuadtree(quadranteTemp->pBase.x, quadranteTemp->pBase.y, w/2,  h/2,nivel+1);
			filhoTemp->filho[0] = filho1;
			filhoTemp->filho[1] = filho2;
			filhoTemp->filho[2] = filho3;
			filhoTemp->filho[3] = filho4;
		}



		return filhoTemp;
	}

}

/// ***********************************************************************
/// ** 
/// ***********************************************************************



void teclado(unsigned char key, int x, int y) {

	switch (key) {
	case 27		: 	exit(0);
		break;				
	case 'q'	:
	case 'Q'	:	raiz = NULL;
		montaQuadtree(0,0,iWidth,iHeight,0);
		break;				
	case 'i'	:
	case 'I'	: 	desenha = !desenha;
		break;				

	case 'b'	:
	case 'B'	: 	
		percent = percent + 5;
		printf("taxa de erro de %.0f %\n",percent);
		break;	

	case 'v'	:
	case 'V'	: 	if(percent > 0)
						percent = percent - 5;
		printf("taxa de erro de %.0f %\n",percent);
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

		case GLUT_LEFT_BUTTON	: 		if(nivel_global < nivel_maximo)
											nivel_global = nivel_global + 1;
			printf("nivel: %d \n",nivel_global);
			break;

		case GLUT_RIGHT_BUTTON	:	if(nivel_global > 0)
										nivel_global = nivel_global - 1;
			printf("nivel: %d \n",nivel_global);
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

	if (desenha)
		desenhaQuadtree(raiz);
	else
		glDrawPixels( iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageGray );

	glutSwapBuffers ();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

int main(int argc, char** argv) {

	char* filename = "PPM/baboon.ppm";

	if (argc > 1)
		filename = argv[1];

	imageGray = leImagem(filename);

	criaJanela(argc, argv);

	initOpenGL();

	initEventos();

	return 0;   
}
