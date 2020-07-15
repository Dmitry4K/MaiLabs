#pragma once
#include<Windows.h>

struct Step {
	int x, y;
};

struct ThParam {
	int **field;
	int player;
	int* count_threads;
	struct Step **answer_buffer;
	int *size_of_buf;
	HANDLE* threads;
	int* used_threads;
};

void ThreadFunctionForTicTacToeBestStep(int **field, int player, int* count_threads, struct Step **answer_buffer, int* size_of_buf);
void ThreadFunctionForTicTacToeLoselessStep(int **field, int player, int* count_threads, struct Step **answer_buffer, int* size_of_buf);
DWORD WINAPI TicTacToeLoselessStepThread(LPVOID LpParam);
void TicTacToeLoselessStep(LPVOID LpParam);
struct ThParam* CopyThreadParam(struct ThParam* Param);
DWORD WINAPI TicTacToeBestStepThread(LPVOID lpParam);
void TicTacToeBestStep(LPVOID lpParam);
//void TicTacToeBestStep(int **field, int player, int* count_threads, struct Step** answer_buffer, int* size_of_buf);
int CheckClearPoint(int**);
int CheckWinner(int**);
void printField(int**);
int** DeleteField(int**);
int** CopyField(int**);
int** CreateField(int**);