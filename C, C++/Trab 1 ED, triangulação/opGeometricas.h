#ifndef __OperacoesGeometricas__
#define __OperacoesGeometricas__ 1

#include <stdio.h>
#include <math.h>

#include "EstruturasDeDados.h"
						
// ***********************************************
// ***********************************************
int		AreaPoly2( void );
bool	InCone( tPonto a, tPonto a1, tPonto a0, tPonto b );

int     Area2( tPonto a, tPonto b, tPonto c );
int     AreaSign( tPonto a, tPonto b, tPonto c );
bool	Xor( bool x, bool y );
bool	Left( tPonto a, tPonto b, tPonto c );
bool	LeftOn( tPonto a, tPonto b, tPonto c );
bool	Collinear( tPonto a, tPonto b, tPonto c );
bool	Between( tPonto a, tPonto b, tPonto c );
bool	Intersect( tPonto a, tPonto b, tPonto c, tPonto d );
bool	IntersectProp( tPonto a, tPonto b, tPonto c, tPonto d );
// ***********************************************
// ***********************************************

#endif	// __OperacoesGeometricas__

