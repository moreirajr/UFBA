#ifndef __QUADTREE__
#define __QUADTREE__ 1
		
void teclado(unsigned char key, int x, int y);

void mouse(int button, int button_state, int x, int y );

void desenho(void);

unsigned char* dividir_img(int w, int h, int nivel);
tNoArvore* NewNode(int nivel, int ponto_x, int ponto_y, int height, int width);
tNoArvore* ramificacao(int nivel, int height, int width, tNoArvore* tree);

#endif	// __QUADTREE__

