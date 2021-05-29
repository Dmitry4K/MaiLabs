% Task 2: Relational Data
%Вариант 1

%-генерирует список List, по заданному предмету Subject
list_of_not_passed_student(Subject, List):-
	bagof(Student, not_passed_subject(Subject, Student), List).

%-подсчет кол-ва студентов в группе не сдавших экзамен
count_of_not_passed_student_in_group(Group, Count):-
	setof(Student, not_passed(Student, Group), List),
	length(List, Count).

%-предикат который находит полную информацию о студенте, а также его оценки за все предметы
sub_predict(Student, Subject ,Mark , Group):-
    grade(Student, Subject ,Mark),
    student(Group, Student).
    
%-Средняя оценка в заданной группе (если группа не задана, ведется подсчет общей средней оценки)
middle_mark(Group, Mark):-
    student(Group, Student),
    findall(Mark,sub_predict(Student, _,Mark,Group),  List),
    sum_list(List, Sum),
    length(List, Length),
    Mark is Sum/Length.

%-предикат поиск несдавшего экзамен студента
not_passed(Student, Group):-
	student(Group, Student),
	grade(Student, _ ,Mark),
	Mark < 3.

%-предикат который ставит в соответствие предмет со студентом, если тот не сдал предмет
not_passed_subject(Subject, Student):-
    grade(Student, Subject, Mark),
    Mark < 3.

% The line below imports the data
:- ['one.pl'].
