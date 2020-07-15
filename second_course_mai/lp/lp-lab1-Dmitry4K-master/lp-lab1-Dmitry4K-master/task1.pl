% Первая часть задания - предикаты работы со списками
% Задание 1

% Расчитать длину списка 
list_length([], 0):-!.
list_length([_Head|Tail], Length):-
    list_length(Tail, TailLength),
    Length is TailLength + 1.

%предикат наличия элемента в списке
list_member(Elem, [Elem|_Tail]).
list_member(Elem, [_Head|Tail]):-
   list_member(Elem, Tail).

%предикат объединение двух списков
list_append([], List2, List2).
list_append([Head|Tail], List2, [Head|Tailresult]):-
   list_append(Tail, List2, Tailresult).

%предикат вложенности списков
sub_start([], _List):-!.
sub_start([Head|TailSub], [Head|TailList]):-
   sub_start(TailSub, TailList).
sublist(Sub, List):-
   sub_start(Sub, List), !.
sublist(Sub, [_Head|Tail]):-
   sublist(Sub, Tail).

%предикат удаления элементов
list_remove(Elem, [Elem|Tail], Tail).
list_remove(Elem, [Head|Tail], [Head|TailResult]):-
    list_remove(Elem, Tail, TailResult).

%перестаовки списков
permute([],[]).
permute(List, [Head|Tail]):-
   list_remove(Head, List, R),
   permute(R, Tail).

% Задание 2
%следующий элемент в списке
list_next(Elem , List, NextElem):-
   list_append(_,[Elem, NextElem|_], List).

list_next_s(Elem, List, NextElem):-
   append(_, [Elem,NextElem|_], List).


% Задание 3
%предикат разделения списка на два по правилу больше-меньше
divide_list(_, [], [],[]):-!.
divide_list(X, [Head|Tail], [Head|Tailb], Listm):-
    Head > X,
    divide_list(X, Tail, Tailb, Listm).
divide_list(X, [Head|Tail], Listb, [Head|Tailm]):-
    divide_list(X, Tail, Listb, Tailm).
