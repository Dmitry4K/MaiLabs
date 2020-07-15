mother-in-law(X,Y):- female(X), female(Y), child(Child, Father) , child(Child, Y), child(Father, X).
father(X,Y):-
    child(Y,X), male(Y).
mother(X,Y):-
    child(Y,X), female(Y).
son(X,Y):-
    child(X,Y), male(Y).
daughter(X,Y):-
    child(X,Y), female(Y).
brother(X,Y):-
    child(P,X),
    child(P,Y),
    male(Y).
sister(X,Y):-
    child(P,X),
    child(P,Y),
    female(Y).

tprolong([X|T], [Y, X|T], father) :-
    father(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], mother) :-
    mother(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], son) :-
    son(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], son) :-
    son(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], daughter) :-
    daughter(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], brother) :-
    brother(X,Y), not(member(Y, [X|T])).
tprolong([X|T], [Y, X|T], sister) :-
    sister(X,Y), not(member(Y, [X|T])).

print_list([]).
print_list([Head|Tail]):-
    write(Head),write(" "),
    print_list(Tail).
next_step(X,Y):-
    child(X,Y).
next_step(X,Y):-
    child(Y,X).
%tprolong([X|T], [Y,X|T]):-
%    next_step(X,Y),
%    not(member(Y,[X|T])).
dfs([X|T], X, [X|T],[]).
dfs(P, F, L, A):-
    tprolong(P,P1, X),
    dfs(P1, F, L,A1),
    append(A1, [X], A).

relative([Z|T], X,Y):-
	!,
    dfs([X],Y,_,A),
       [Z|T] = A,!.
   % print_list(List).
relative(Z,X,Y):-
  dfs([X],Y,_,A),
  [Z] = A,!.

female('@I500002@').
female('@I500007@').
female('@I500008@').
female('@I500010@').
female('@I500012@').
female('@I500014@').
female('@I500017@').
female('@I500018@').
female('@I500019@').
female('@I500026@').
female('@I500031@').
female('@I500035@').
female('@I500036@').
female('@I500038@').
female('@I500043@').
female('@I500046@').
female('@I500048@').
female('@I500050@').
female('@I500052@').
female('@I500057@').
female('@I500058@').
female('@I500061@').
female('@I500063@').
female('@I500067@').
female('@I500070@').
female('@I500074@').
female('@I500076@').
male('@I500001@').
male('@I500003@').
male('@I500004@').
male('@I500005@').
male('@I500006@').
male('@I500009@').
male('@I500011@').
male('@I500013@').
male('@I500015@').
male('@I500016@').
male('@I500020@').
male('@I500021@').
male('@I500022@').
male('@I500023@').
male('@I500024@').
male('@I500025@').
male('@I500027@').
male('@I500028@').
male('@I500029@').
male('@I500030@').
male('@I500032@').
male('@I500033@').
male('@I500034@').
male('@I500037@').
male('@I500039@').
male('@I500040@').
male('@I500041@').
male('@I500042@').
male('@I500044@').
male('@I500045@').
male('@I500047@').
male('@I500049@').
male('@I500051@').
male('@I500053@').
male('@I500060@').
male('@I500062@').
male('@I500064@').
male('@I500065@').
male('@I500066@').
male('@I500068@').
male('@I500069@').
male('@I500071@').
male('@I500072@').
male('@I500073@').
male('@I500075@').
child('@I500003@', '@I500001@').
child('@I500004@', '@I500001@').
child('@I500003@', '@I500002@').
child('@I500004@', '@I500002@').
child('@I500001@', '@I500006@').
child('@I500005@', '@I500006@').
child('@I500008@', '@I500006@').
child('@I500001@', '@I500007@').
child('@I500005@', '@I500007@').
child('@I500008@', '@I500007@').
child('@I500010@', '@I500009@').
child('@I500011@', '@I500009@').
child('@I500010@', '@I500008@').
child('@I500011@', '@I500008@').
child('@I500002@', '@I500015@').
child('@I500002@', '@I500012@').
child('@I500012@', '@I500013@').
child('@I500017@', '@I500013@').
child('@I500018@', '@I500013@').
child('@I500019@', '@I500013@').
child('@I500020@', '@I500013@').
child('@I500021@', '@I500013@').
child('@I500012@', '@I500014@').
child('@I500017@', '@I500014@').
child('@I500018@', '@I500014@').
child('@I500019@', '@I500014@').
child('@I500020@', '@I500014@').
child('@I500021@', '@I500014@').
child('@I500028@', '@I500025@').
child('@I500015@', '@I500025@').
child('@I500027@', '@I500025@').
child('@I500028@', '@I500026@').
child('@I500015@', '@I500026@').
child('@I500027@', '@I500026@').
child('@I500033@', '@I500031@').
child('@I500034@', '@I500031@').
child('@I500033@', '@I500032@').
child('@I500034@', '@I500032@').
child('@I500006@', '@I500037@').
child('@I500006@', '@I500038@').
child('@I500047@', '@I500045@').
child('@I500048@', '@I500045@').
child('@I500049@', '@I500045@').
child('@I500050@', '@I500045@').
child('@I500047@', '@I500046@').
child('@I500048@', '@I500046@').
child('@I500049@', '@I500046@').
child('@I500050@', '@I500046@').
child('@I500041@', '@I500042@').
child('@I500041@', '@I500043@').
child('@I500057@', '@I500005@').
child('@I500058@', '@I500005@').
child('@I500057@', '@I500052@').
child('@I500058@', '@I500052@').
child('@I500060@', '@I500045@').
child('@I500062@', '@I500045@').
child('@I500060@', '@I500061@').
child('@I500062@', '@I500061@').
child('@I500063@', '@I500064@').
child('@I500065@', '@I500064@').
child('@I500063@', '@I500048@').
child('@I500065@', '@I500048@').
child('@I500067@', '@I500068@').
child('@I500070@', '@I500068@').
child('@I500067@', '@I500063@').
child('@I500070@', '@I500063@').
child('@I500072@', '@I500071@').
child('@I500073@', '@I500071@').
child('@I500072@', '@I500050@').
child('@I500073@', '@I500050@').


def_name_by_id('@I500001@', 'Василий Коростелев').
def_name_by_id('@I500002@', 'Елена Чистоногова').
def_name_by_id('@I500003@', 'Дмитрий Коростелев').
def_name_by_id('@I500004@', 'Александр Коростелев').
def_name_by_id('@I500005@', 'Александр Коростелев').
def_name_by_id('@I500006@', 'Николай Коростелев').
def_name_by_id('@I500007@', 'Зинаида Чеснокова').
def_name_by_id('@I500008@', 'Татьяна Коростелева').
def_name_by_id('@I500009@', 'Михаил Перцов').
def_name_by_id('@I500010@', 'Ирина Перцов').
def_name_by_id('@I500011@', 'Александр Перцов').
def_name_by_id('@I500012@', 'Татьяна Чистоногова').
def_name_by_id('@I500013@', 'Гаврил Белоусов').
def_name_by_id('@I500014@', 'Агафья Руднева').
def_name_by_id('@I500015@', 'Анатолий Малыхин').
def_name_by_id('@I500016@', 'Виктор Чистоногов').
def_name_by_id('@I500017@', 'Антонина Фоменко').
def_name_by_id('@I500018@', 'Нина Белоусова').
def_name_by_id('@I500019@', 'Валентина Белоусова').
def_name_by_id('@I500020@', 'Виктор Белоусова').
def_name_by_id('@I500021@', 'Иван Белоусов').
def_name_by_id('@I500022@', 'Петр Белоусов').
def_name_by_id('@I500023@', 'Максим Руднев').
def_name_by_id('@I500024@', 'Иван Руднев').
def_name_by_id('@I500025@', 'Петр Малыхин').
def_name_by_id('@I500026@', 'Раиса Жигулина').
def_name_by_id('@I500027@', 'Александр Малыхин').
def_name_by_id('@I500028@', 'Николай Малыхин').
def_name_by_id('@I500029@', 'Николай Малыхин').
def_name_by_id('@I500030@', 'Афанасий Жигулин').
def_name_by_id('@I500031@', 'Полина Жигулина').
def_name_by_id('@I500032@', 'Андрей Жигулина').
def_name_by_id('@I500033@', 'Петр Жигулина').
def_name_by_id('@I500034@', 'Василий Жигулин').
def_name_by_id('@I500035@', 'Полина Малыхина').
def_name_by_id('@I500036@', 'Мария Бредихина').
def_name_by_id('@I500037@', 'Иван Коростелев').
def_name_by_id('@I500040@', 'Ивае Коростелев').
def_name_by_id('@I500041@', 'Иван Чесноков').
def_name_by_id('@I500042@', 'Иван Чесноков').
def_name_by_id('@I500045@', 'Александр Чесноков').
def_name_by_id('@I500046@', 'Мария Чеснокова').
def_name_by_id('@I500047@', 'Василий Чесноков').
def_name_by_id('@I500048@', 'Лидия Чеснокова').
def_name_by_id('@I500049@', 'Николай Чесноков').
def_name_by_id('@I500050@', 'Нина Чеснокова').
def_name_by_id('@I500051@', 'Петр Перцов').
def_name_by_id('@I500052@', 'Надежда Миркулова').
def_name_by_id('@I500053@', 'Владимир Миркулова').
def_name_by_id('@I500057@', 'Наталья Коростелева').
def_name_by_id('@I500058@', 'Ольга Коростелева').
def_name_by_id('@I500060@', 'Сергей Чесноков').
def_name_by_id('@I500061@', 'Раиса Чеснокова').
def_name_by_id('@I500062@', 'Владимир Чесноков').
def_name_by_id('@I500063@', 'Надежда Чеснокова').
def_name_by_id('@I500064@', 'Валентин Игнатьев').
def_name_by_id('@I500065@', 'Александр Игнатьев').
def_name_by_id('@I500067@', 'Татьяна Вишнякова').
def_name_by_id('@I500068@', 'Анатолий Вишняков').
def_name_by_id('@I500069@', 'Иван Вишняков').
def_name_by_id('@I500070@', 'Светлана Вишнякова').
def_name_by_id('@I500071@', 'Александр Завардин').
def_name_by_id('@I500072@', 'Владимир Завардин').
def_name_by_id('@I500073@', 'Роман Завардин').
def_name_by_id('@I500074@', 'Раиса Никитина').
def_name_by_id('@I500075@', 'Валерий Чесноков').
def_name_by_id('@I500076@', 'Галина Чеснокова').
