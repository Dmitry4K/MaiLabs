#№ Отчет по лабораторной работе №4
## по курсу "Логическое программирование"

## Обработка естественного языка

### студент:Коростелев Д.В.

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|              |               |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*


## Введение

Проблема обработки языков машиной – одна из первостепенных на данный момент. Научить машину понимать язык – крайне сложная, пока что невыполнимая задача, так как перед программистом стоит главный вопрос – как сопоставить передаваемый смысл с предложением. Однако уже сейчас многие компании разрабатывают свои алгоритмы обработки языков, которые справляются со своей задачей довольно успешно. В реализации таких алгоритмов огромную роль играют свойства, присущие тому или иному языку, так как они структурированы и могут быть запрограммированы. 
Грамматика языка – свойство того или иного языка, почти все алгоритмы обработки языков основаны на грамматиках и именно в зависимости от типа грамматики выбирается и способ обработки языка – так для регулярных автоматик применяется метод конечных автоматов, для контекстно-свободной – строится дерево грамматического разбора и т.д.
Язык пролог хорош для обработки языка тем, что в нем довольно легко сопоставлять различные состояния и правила, можно с легкостью находить различные паттерны, анализировать и редактировать текст.


## Задание

Реализовать разбор предложений английского языка. В предложениях у объекта (подлежащего) могут быть заданы цвет, размер, положение. В результате разбора должны получится структуры представленные в примере. 

```prolog
?- sentence(["the", "big", "book","is","on","the","table"],X).
X = s(location(object("book", size("big")), on("table"))).

?- sentence(["the", "red", "book","on","the","table", "is", "blue"],X).
X = s(object(location("book", on("table")), color("red")), color("blue")).
```
## Принцип решения

Принцип решения данной задачи можно разделить на несколько шагов.\
Первым  делом задаются словари существительных, глаголов, предлогов, цвета и предлоги, отвечающие за положения объекта относительно другого объекта. 

```prolog
verbs(["is","are","am"]).
nouns(["book", "table", "box"]).
colors(["red","blue","green","black","white","orange"]).
sizes(["big", "small", "litle", "middle", "huge"]).
pretexts(["on","in","into"]).
...
```
Далее вводим логику по которой будет раскладываться предложение:
Если встретили комбинацию on the создаем структуру on, содержанием которой будет ответ найденный при просмотре следующего за ним слова (Аналогично для остальных предлогов).
Если встретили комбинацию the Сolor Если создаем структуру color(Color) продолжаем разбор (аналогично для других характеристик).
Если встретили комбинацию the Elem, где Elem – существительное, то возвращаем Elem. 
Если встретили комбинацию Elem on - создаем структуру location(Elem, on(Answer)), Answer – ищется в следующем проходе (Аналогично для остальных предлогов).
Если в конце встретилось одно слово – существительное, возвращаем его (Аналогичное действие для цветов и размеров, только еще создается соответствующая структура).

```prolog
...

check_sentence(["on","the"|Tail], on(Answer)):-
    check_sentence(Tail, Answer).
check_sentence(["in","the"|Tail], in(Answer)):-
    check_sentence(Tail, Answer).
check_sentence(["into","the"|Tail], into(Answer)):-
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
check_sentence([Elem], color(Elem)):-
    colors(Colors),
    member(Elem, Colors).
check_sentence([Elem], size(Elem)):-
    sizes(Sizes),
    member(Elem, Sizes).
...
```

Далее реализация главного предиката. По заданию требуется разобрать предложение, поэтому логичным в нем будет найти глагол, далее по глаголу разделить предложение с помощью предиката конкатенации двух списков и элемента. 

```prolog
...
list_append([], Elem, List2, [Elem|List2]).
list_append([Head|Tail], Elem, List2, [Head|TailRes]):-
    list_append(Tail, Elem,List2, TailRes).
...
```

Получив два списка – левая часть предложения и правая, 
Начинаем разбор обеих и в зависимости от того какой элемент стоит первый в правом списке, создаем соответствующие структуры.

```prolog
...
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
```
## Результаты
```prolog
?- sentence(["the", "big", "book","is","on","the","table"],X).
X = s(location(object("book", size("big")), on("table"))).

?- sentence(["the", "red", "book","in","the","table", "is", "blue"],X).
X = s(object(location("book", in("table")), color("red")), color("blue")).

?- sentence(["the", "red", "book","into","the","table", "is", "blue"],X).
X = s(object(location("book", into("table")), color("red")), color("blue")).

?- sentence(["the", "big", "book","is","on","the","table"],X).
X = s(location(object("book", size("big")), on("table"))).

?- sentence(["the", "big", "book","is","in","the","table"],X).
X = s(location(object("book", size("big")), in("table"))).

?- sentence(["the", "red", "book","is","on","the","table"],X).
X = s(location(object("book", color("red")), on("table"))).

?- sentence(["the", "red", "book","is","big"],X).
X = s(object("book", color("red")), size("big"))
```

Как видим предикат выдает предложение, представленное в структурной форме, в которой может быть представлен размер, цвет и положение объекта.
## Выводы

Благодаря данной лабораторной работе познакомился с основными принципами обработки естественных языков, реализовал предикат, который разбирает простые предложения английского языка по составу. Хотелось бы отметить, что сам по себе процесс обработки языка довольно широкий и наполненный, в нем присутствует множество нюансов и особенных условий, которые бывает очень сложно реализовать с помощью программного кода, но стоит отметить, что даже с этой задачей Prolog справляется довольно хорошо (процесс написания программ для обработки текста занимает не так много сил и времени, если говорить о базовой обработке). В этом можно убедиться посмотрев на решение моей задачи – благодаря нескольким правилам, получилось переформировать предложение в структуру, кроме того, данный алгоритм может быть спокойно расширен и наполнен новыми фразами, словами, конструкциями и так далее.
