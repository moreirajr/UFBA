% Autor:  Evaldo M. Moreira Junior
% Data: 18/12/2012


particiona([], [], []).
particiona([X], [X], []).
particiona([X,Y|Z], [X|L], [Y|R]):- X=< Y, !, particiona(Z, L, R).
particiona([X,Y|Z], [Y|L], [X|R]):- X > Y, !, particiona(Z, L, R).

junte([],[],[]).
junte([],[X|L],[X|L]).
junte([X|L],[], [X|L]).
junte([X|L], [Y|R], [X|W]):-  X=<Y, junte(L,[Y|R],W).
junte([X|L], [Y|R], [Y|W]):-  X >Y, junte([X|L],R,W).


quicksort([], []).
quicksort([X], [X]).
quicksort([X, Y|Z], W):- particiona([X,Y|Z], U, V),
                        quicksort(U,Q), quicksort(V,R), junte(Q,R,W).
                        

