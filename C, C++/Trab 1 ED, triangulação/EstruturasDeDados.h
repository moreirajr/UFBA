#ifndef __EstruturasDeDados__
#define __EstruturasDeDados__ 1


typedef enum { FALSE, TRUE } bool;

typedef struct point 	{	int x;
							int y;
						} tPonto;
						
typedef struct	{ 	int 	ID;
					bool 	orelha;
					tPonto	p;
				} tVertice;
								
#endif		// __EstruturasDeDados__
