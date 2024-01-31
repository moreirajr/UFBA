% Autor:  Evaldo M. Moreira Junior
% Data: 14/12/2012


% Escreva a rotina Merge(L1, L2, L) onde L1,L2,L são Listas ordenadas.

merge([], [], []).
merge([X|L1], [], [X|L1]).
merge([], [Y|L2], [Y|L2]).
merge([X|L1],[Y|L2],[X|L]) :- X<Y, !, merge(L1, [Y|L2], L).
merge([X|L1],[Y|L2],[Y|L]) :- X>Y, !, merge([X|L1], L2, L).
merge([Y|L1],[Y|L2],[Y|L]) :- merge(L1, L2, L).

