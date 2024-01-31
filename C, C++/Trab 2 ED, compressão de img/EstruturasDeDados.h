#ifndef __EstruturasDeDados__
#define __EstruturasDeDados__ 1

#define MAX(a,b) (a) > (b) ? (a) : (b)


typedef struct point 	{	int x;
							int y;
						} tPonto;
						
typedef struct	{ 	int 	h,w;
					int 	nivel;
					tPonto	pBase;
					int		cor;
					int eFolha;
				} tQuadrante;
				
typedef struct tNo	{	tQuadrante* q;
						struct tNo* filho[4];
					} tNoArvore;
								
#endif		// __EstruturasDeDados__
