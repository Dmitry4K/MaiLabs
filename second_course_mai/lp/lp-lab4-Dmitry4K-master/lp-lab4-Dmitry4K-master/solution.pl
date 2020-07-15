verbs(["is","are","am"]).
nouns(["book", "table", "box"]).
colors(["red","blue","green","black","white","orange"]).
sizes(["big", "small", "litle", "middle", "huge"]).
pretexts(["on","in","into"]).
list_append([], Elem, List2, [Elem|List2]).
list_append([Head|Tail], Elem, List2, [Head|TailRes]):-
    list_append(Tail, Elem,List2, TailRes).

check_sentence(["on","the"|Tail], on(Answer)):-
    check_sentence(Tail, Answer).
check_sentence(["the", Elem1|Tail], object(Answer, color(Elem1))):-
    colors(Colors),
    member(Elem1, Colors),
    check_sentence(Tail, Answer).
check_sentence(["the", Elem1|Tail], object(Answer, size(Elem1))):-
    sizes(Sizes),
    member(Elem1, Sizes),
    check_sentence(Tail, Answer).
check_sentence(["the", Elem], Elem):-
    nouns(Nouns),
    member(Elem, Nouns).
   % check_sentence(Tail, Elem).
check_sentence([Elem, "on"|Tail], location(Elem, on(Answer))):-
    nouns(Nouns),
    member(Elem, Nouns),
    check_sentence(Tail, Answer).
check_sentence([Elem, "in"|Tail], location(Elem, in(Answer))):-
    nouns(Nouns),
    member(Elem, Nouns),
    check_sentence(Tail, Answer).
check_sentence([Elem, "into" | Tail], location(Elem, into(Answer))):-
    nouns(Nouns),
    member(Elem, Nouns),
    check_sentence(Tail, Answer).
check_sentence([Elem], Elem):-
    nouns(Nouns),
    member(Elem, Nouns).

check_sentence(["on"|Tail], on(Answer)):-
    check_sentence(Tail, Answer).
check_sentence([Elem], color(Elem)):-
    colors(Colors),
    member(Elem, Colors).
check_sentence([Elem], size(Elem)):-
    sizes(Sizes),
    member(Elem, Sizes).

sentence(List, s(location(La,Ra))):-
   %----находим глагол в предложении----
   verbs(Verbs),
   member(Verb, Verbs),
   member(Verb, List),
   %----делим предложение по глаголу----
   list_append(Left, Verb, Right, List),
  % append(LeftLeft, [X], Left),
  Right = [Rhead|_],
  pretexts(Pretexts),
  member(Rhead, Pretexts),
  check_sentence(Left, La),
   check_sentence(Right, Ra),!.
sentence(List, s(La,Ra)):-
   %----находим глагол в предложении----
   verbs(Verbs),
   member(Verb, Verbs),
   member(Verb, List),
   %----делим предложение по глаголу----
   list_append(Left, Verb, Right, List),
   Right = [Rhead|_],
   pretexts(Pretexts),
   not(member(Rhead, Pretexts)),
   check_sentence(Left, La),
   check_sentence(Right, Ra),!.
