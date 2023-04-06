   #define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<Windows.h>
#include<tchar.h>
#include<locale.h>
#include<time.h>
#include"GameTicTacToe.h"



int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	int** Field = NULL;
	Field = CreateField(Field);
	printf("������� ���� 3 x 3 :\n");
	printf("  v v v\n");

	for (int i = 0; i < 3; i++) {
		printf("> ");
		for (int j = 0; j < 3; j++)
			scanf("%d", &Field[i][j]);
	}

	printf("\n������� ������, ������� ����� ��������� : ");
	int Player;
	scanf("%d", &Player);

	printf("������� ���-�� ������� ��������� ��� �������� : ");
	int ThreadCount;
	scanf("%d", &ThreadCount);

	HANDLE* Threads = malloc(sizeof(HANDLE)*ThreadCount);
	struct Step* Answers = NULL;
	int* AnswersSize = (int*)malloc(sizeof(int));
	*AnswersSize = 0;
	//TicTacToeBestStep(Field, Player, &ThreadCount, &Answers, AnswersSize);
	clock_t start = clock();
	ThreadFunctionForTicTacToeBestStep(Field, Player, &ThreadCount, &Answers, AnswersSize);
	printf("\n");
	if (*AnswersSize == 0) {
		printf("���������� ����� ���\n");
		ThreadFunctionForTicTacToeLoselessStep(Field, Player, &ThreadCount, &Answers, AnswersSize);
		for (int i = 0; i < *AnswersSize; i++)
			printf("�������������� ��� : %d %d\n", Answers[i].x, Answers[i].y);
		
	}
	else {
		for (int i = 0; i < *AnswersSize; i++)
			printf("���������� ��� : %d %d\n", Answers[i].x, Answers[i].y);
	}
	clock_t end = clock();
	printf("\n����� ����������� �� ������� : %.0lf ��\n\n", (end - start) * 1000.0 / (CLOCKS_PER_SEC));

	system("pause");
	return 0;
}