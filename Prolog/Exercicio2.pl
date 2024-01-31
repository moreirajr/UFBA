% Autor:  Evaldo M. Moreira Junior
% Data: 12/12/2012

%    Escreva a função quadradoMenor( X, L1, L2) que retorna em L2 todos os elementos de L1 cujo quadrado é menor que X.


membro(X, [X|R]).
membro(X, [Y|R]):- membro(X, R).

remova(X, [X|R], R).
remova(X, [F|R], [F|S]):- remova (X, R, S).

insere(X,L,R):- remova(X,R,L).

quadradoMenor(X, L1, L2) :- membro(Z, L1), Y is Z*Z, Y < X, insere(Z, [], L2).
