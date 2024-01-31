% Autor:  Evaldo M. Moreira Junior
% Data: 19/12/2012


%Questao 1

concatenar([], [], []).
concatenar([X|L], [], [X|L]).
concatenar([], [X|L], [X|L]).
concatenar([X|A], [Y|B], [X|W]):- concatenar(A, [Y|B], W).

%Questao 2

concatenarInv([], [], []).
concatenarInv([X|L], [], [X|L]).
concatenarInv([], [X|L], [X|L]).
concatenarInv([X|A], [Y|B], [Y|W]):- concatenarInv([X|A], B, W).

%Questao 3

listaMembro([], [[]]):- !.
listaMembro(L, [L|R]).
listaMembro(L, [T|R]):- listaMembro(L, R).
%errado
concatenar([[]],X):- !.
concatenar(L, R):- listaMembro(S, L), concatenar(S, [], R).

%Questao 4

juntar([],[],[]).
juntar([X|L], [], [X|L]).
juntar([], [Y|L], [Y|L]).
juntar([X|I], [Y|J], [X,Y|K]):- juntar(I,J,K).

%Questao 5

adicionarFinal(X, [], [X]).
adicionarFinal(X, [Y], [Y|X]).
adicionarFinal(X, [Y|Z], [Y|W]):- adicionarFinal(X, Z, W).

%Questao 6

inverter([], []).
inverter([X|Y], W):- inverter(Y, K), concatenar(K, [X], W).

%Questao 7

parear(_, [],[]).
parear(X, [Y|Z], [[X,Y]|L]):- parear(X, Z, L).

%Questao 8

pares([],[]).
pares([X|L], P):- concatenar(W, Y, P), parear(X, L, W), pares(L, Y).

%Questao 9

remova(X, [X|T], T).
remova(X, [H|T], [H|NT]):- remova(X,T,NT).

permutar([],[]).
permutar([X|L], P):- permutar(L, R), remova(X, P, R).

%Questao 10

membro(X, [X|_]):- !.
membro(X, [Y|R]):- membro(X, R).

comparacao([]).      %remover = erro, \+ (negacao)
comparacao([X|Z]):-  \+ membro(X,Z), comparacao(Z).

conjunto([]):- !.
conjunto([X]):- !.
conjunto([X|Y]):- comparacao([X|Y]).

%Questao 11

prefixo([], []).
prefixo([X], [X|L]):- !.
prefixo([X|L], [X|L]):- !.
prefixo([X|L], [X|R]):- prefixo(L,R),!.

%Questao 12
%L subsequencia de R se R pode ser decomposta em R1 / R2 e R2 pode ser decomposta em L / R3 ,  R = R1 L R3
subsequencia(L, R):- concatenar(R1, R2, R), concatenar(L, R3, R2),!.