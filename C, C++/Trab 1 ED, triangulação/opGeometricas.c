#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opGeometricas.h"


// ***********************************************
// Verifica se a aresta (a,b) Ž interna na vizinhaca dos pontos a1 e a0
// onde a1 e o vertice anterior e a0 o vertice posterior ao vertice a.
// ***********************************************

bool   InCone( 	tPonto a, tPonto a1, tPonto a0, tPonto b ) {

   /* If a is a convex vertex ... */
   if( LeftOn( a, a1, a0 ) )
       return    Left( a, b, a0 )
              && Left( b, a, a1 );

   /* Else a is reflex: */
       return !(    LeftOn( a, b, a1 )
                 && LeftOn( b, a, a0 ) );
}

// ***********************************************
// ***********************************************
int     Area2( tPonto a, tPonto b, tPonto c ) {
   return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

// ***********************************************
// ***********************************************
bool	Xor( bool x, bool y ) {
   return   !x ^ !y;
}

// ***********************************************
// Verifica se (ab) intesepta (c,d) 
// ***********************************************
bool	IntersectProp( tPonto a, tPonto b, tPonto c, tPonto d ) {
   /* Elimina casos improprios. */
   if (
      Collinear(a,b,c) ||
      Collinear(a,b,d) ||
      Collinear(c,d,a) ||
      Collinear(c,d,b)
      )
      return FALSE;

   return
         Xor( Left(a,b,c), Left(a,b,d) )
      && Xor( Left(c,d,a), Left(c,d,b) );
}

// ***********************************************
// retorna TRUE se o ponto c esta a esquerda de (a,b)
// ***********************************************
bool	Left( tPonto a, tPonto b, tPonto c ) { 
   return  AreaSign( a, b, c ) > 0;
}

// ***********************************************
// ***********************************************
bool	LeftOn( tPonto a, tPonto b, tPonto c ) {
   return  AreaSign( a, b, c ) >= 0;
}

// ***********************************************
// ***********************************************
bool	Collinear( tPonto a, tPonto b, tPonto c ) {
   return  AreaSign( a, b, c ) == 0;
}

// ***********************************************
// Verifica se c Ž interno ao segmento (a,b)
// ***********************************************
bool	Between( tPonto a, tPonto b, tPonto c ) {
   tPonto	ba, ca;

   if ( ! Collinear( a, b, c ) )
      return  FALSE;

   if ( a.x != b.x ) 
      return ((a.x <= c.x) && (c.x <= b.x)) ||
             ((a.x >= c.x) && (c.x >= b.x));
   else
      return ((a.y <= c.y) && (c.y <= b.y)) ||
             ((a.y >= c.y) && (c.y >= b.y));
}

// ***********************************************
// Retorna TRUE caso (a,b) intersepte (c,d), mesmo que de forma impropria
// ***********************************************
bool	Intersect( tPonto a, tPonto b, tPonto c, tPonto d ) {
   if      ( IntersectProp( a, b, c, d ) )
      return  TRUE;
   else if (   Between( a, b, c )
            || Between( a, b, d )
            || Between( c, d, a )
            || Between( c, d, b )
           )
      return  TRUE;
   else    return  FALSE;
}

// ***********************************************
// ***********************************************
int     AreaSign( tPonto a, tPonto b, tPonto c ) {
    double area2;

    area2 = ( b.x - a.x ) * (double)( c.y - a.y ) -
            ( c.x - a.x ) * (double)( b.y - a.y );

    /* arredondamento para inteiro. */
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}


