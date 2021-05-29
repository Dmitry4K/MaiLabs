remove_element_by_index([Head|Tail], Tail, 1, Head).
remove_element_by_index([Head|Tail], [Head|ResTail] , Index, Deleted):-
    Index \= 1,
    NewIndex is Index - 1,
    remove_element_by_index(Tail, ResTail, NewIndex, Deleted).

next_step(step(Left, Right), step(ResLeft, ResRight)):-
    length(Left, LeftLength),
    remove_element_by_index(Left, ResLeft, LeftLength, Elem),
    ResRight = [Elem|Right].
next_step(step(Left, Right), step(ResLeft, ResRight)):-
    Right = [Head|ResRight],
    append(Left, [Head], ResLeft).
next_step(step(Left, Right), step(ResLeft, ResRight)):-
    length(Left, LeftLength),
    PreIndex is LeftLength - 1,
    remove_element_by_index(Left, ResLeft, PreIndex, Deleted),
    ResRight = [Deleted|Right].
next_step(step(Left, Right), step(ResLeft, ResRight)):-
    remove_element_by_index(Right, ResRight, 2, Deleted),
    append(Left, [Deleted], ResLeft).


tprolong([X|T], [Y, X|T]):-
    next_step(X,Y),
    not(member(Y, [X|T])).
dfs([X|T], X, [X|T]).
dfs(P, F,L):-
    tprolong(P,P1),
    dfs(P1, F,L).
print_step(step(X,Y)):-
    write(X),
    write(" "),
    write(Y),
    nl.
print_list([]).
print_list([Head|Tail]):-
    print_step(Head),
    print_list(Tail).

task3():-
    get_time(Start),
    dfs([step([black, black, black], [white, white, white])], step([white,white,white],[black,black,black]), List),
    get_time(End),
    Time is End - Start,
    reverse(List, Result),
    print_list(Result),
    write("Time to solve: "), write(Time), nl,
    length(Result, Length),
    write("The count of steps: "), write(Length), nl.

