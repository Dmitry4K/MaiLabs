#include"GameTicTacToe.h"
#include<malloc.h>

CRITICAL_SECTION section = { 0 };
/*
struct Step* CalculateBestStep(int ** Field, int Player) {
	struct Step* res = NULL;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (Field[i][j] != 2 && Field[i][j] != 1) {
				Field[i][j] = Player;
				//printField(Field);
				//printf("%d\n", i);
				if (CheckWinner(Field) == Player) {
					//printf("winner: %d\n", Player);
					res = (struct Step*)malloc(sizeof(struct Step));
					res->x = i;
					res->y = j;
					return res;
				}
				int ** FieldC = CopyField(Field);
				res = CalculateBestStep(FieldC, (Player * 2) % 3);
				FieldC = DeleteField(FieldC);
				if (res == NULL) {
					res = (struct Step*)malloc(sizeof(struct Step));
					res->x = i;
					res->y = j;
					return res;
				}
				Field[i][j] = 0;
			}
	return res;
}*/

void printField(int** Field) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			printf("%d ", Field[i][j]);
		printf("\n");
	}
}

int** DeleteField(int** Field) {
	for (int i = 0; i < 3; i++)
		free(Field[i]);
	free(Field);
	return NULL;
}

int** CopyField(int** Field) {
	int** res = (int**)malloc(sizeof(int*) * 3);
	for (int i = 0; i < 3; i++)
		res[i] = (int*)malloc(sizeof(int) * 3);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res[i][j] = Field[i][j];
	return res;
}

int CheckWinner(int ** Field) {
	for (int i = 0; i < 3; i++) {
		if (Field[i][0] == Field[i][1] && Field[i][0] == Field[i][2] && Field[i][0] != 0)
			return Field[i][0];
		if (Field[0][i] == Field[1][i] && Field[0][i] == Field[2][i] && Field[0][i] != 0)
			return Field[0][i];
	}
	if (Field[0][0] == Field[2][2] && Field[1][1] == Field[2][2] && Field[0][0] != 0)
		return Field[0][0];
	if (Field[1][1] == Field[0][2] && Field[1][1] == Field[2][0] && Field[1][1] != 0)
		return Field[1][1];
	return 0;
}


int** CreateField(int** Field) {
	Field = (int **)malloc(sizeof(int*) * 3);
	for (int i = 0; i < 3; i++)
		*(Field + i) = (int *)malloc(sizeof(int) * 3);
	return Field;
}

DWORD WINAPI TicTacToeBestStepThread(LPVOID LpParam) {
	struct ThParam *ThreadParam = (struct ThParam*)LpParam;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (ThreadParam->field[i][j] == 0) {
				int** c_field = CopyField(ThreadParam->field);
				c_field[i][j] = ThreadParam->player;
				//printField(c_field);
				if (CheckWinner(c_field) == ThreadParam->player) {
					if (*(ThreadParam->answer_buffer) != NULL)
						*(ThreadParam->answer_buffer) = (struct Step*)realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
					else
						*(ThreadParam->answer_buffer) = (struct Step*)malloc(sizeof(struct Step));
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
					(*(ThreadParam->size_of_buf))++;
				}
				else {
					struct Step* answer_buffer_next = NULL;
					int* abf_size = (int*)malloc(sizeof(int));
					*abf_size = 0;
					struct ThParam * c_LpParam = CopyThreadParam(ThreadParam);
					c_LpParam->field = c_field;
					c_LpParam->player = (ThreadParam->player * 2) % 3;
					//c_LpParam->count_threads = 
					c_LpParam->answer_buffer = &answer_buffer_next;
					c_LpParam->size_of_buf = abf_size;
					int cur_thread = 0;
					if (*(c_LpParam->used_threads) < *(c_LpParam->count_threads)) {
					//	printf("1");
						//printf("%d ", *((*c_LpParam).used_threads));

						//EnterCriticalSection(&section);
						*(c_LpParam->used_threads) = *(c_LpParam->used_threads) + 1;
						//LeaveCriticalSection(&section);
						c_LpParam->threads[*(c_LpParam->used_threads)-1] = CreateThread(NULL, 0, TicTacToeBestStepThread, c_LpParam, 0, NULL);

						cur_thread = *(c_LpParam->used_threads);
					}
					else {
						TicTacToeBestStep((LPVOID)c_LpParam);
					}
					if (cur_thread != 0)
						WaitForSingleObject(ThreadParam->threads[cur_thread - 1], INFINITE);
					if (*abf_size == 0 && CheckClearPoint(c_field) > 0) {
						if (*(ThreadParam->answer_buffer) != NULL)
							*(ThreadParam->answer_buffer) = realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
						else
							*(ThreadParam->answer_buffer) = malloc(sizeof(struct Step));
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
						(*(ThreadParam->size_of_buf))++;
					}
				}
				c_field = DeleteField(c_field);
			}
		}
	}
	ExitThread(0);
}

void TicTacToeBestStep(LPVOID LpParam) {
	struct ThParam *ThreadParam = (struct ThParam*)LpParam;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (ThreadParam->field[i][j] == 0) {
				int** c_field = CopyField(ThreadParam->field);
				c_field[i][j] = ThreadParam->player;
				//printField(c_field);
				if (CheckWinner(c_field) == ThreadParam->player) {
					if (*(ThreadParam->answer_buffer) != NULL)
						*(ThreadParam->answer_buffer) = (struct Step*)realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf)+1) * sizeof(struct Step));
					else
						*(ThreadParam->answer_buffer) = (struct Step*)malloc(sizeof(struct Step));
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
					(*(ThreadParam->size_of_buf))++;
				}
				else {
					struct Step* answer_buffer_next = NULL;
					int* abf_size = (int*)malloc(sizeof(int));
					*abf_size = 0;
					struct ThParam * c_LpParam = CopyThreadParam(ThreadParam);
					c_LpParam->field = c_field;
					c_LpParam->player = (ThreadParam->player * 2) % 3;
					//c_LpParam->count_threads = 
					c_LpParam->answer_buffer = &answer_buffer_next;
					c_LpParam->size_of_buf = abf_size;
					TicTacToeBestStep((LPVOID)c_LpParam);
					if (*abf_size == 0 && CheckClearPoint(c_field) > 0) {
						if (*(ThreadParam->answer_buffer) != NULL)
							*(ThreadParam->answer_buffer) = realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf)+1) * sizeof(struct Step));
						else
							*(ThreadParam->answer_buffer) = malloc(sizeof(struct Step));
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
						(*(ThreadParam->size_of_buf))++;
					}
				}
				c_field = DeleteField(c_field);
			}
		}
	}
}


void ThreadFunctionForTicTacToeBestStep(int **field, int player, int* count_threads, struct Step **answer_buffer, int* size_of_buf) {
	InitializeCriticalSection(&section);
	int*  used_threads = malloc(sizeof(int));
	*used_threads = 0;
	HANDLE* Threads = malloc(sizeof(HANDLE)*(*count_threads));
	struct ThParam*  ThreadParam = malloc(sizeof(struct ThParam));
	ThreadParam->field = field;
	ThreadParam->player = player;
	ThreadParam->count_threads = count_threads;
	ThreadParam->answer_buffer = answer_buffer;
	ThreadParam->size_of_buf = size_of_buf;
	ThreadParam->threads = Threads;
	ThreadParam->used_threads = used_threads;
	if (*count_threads > 0) {

		//EnterCriticalSection(&section);
		*(ThreadParam->used_threads) = *(ThreadParam->used_threads) + 1;
		//LeaveCriticalSection(&section);
		ThreadParam->threads[*(ThreadParam->used_threads) - 1] = CreateThread(NULL, 0, TicTacToeBestStepThread, ThreadParam, 0, NULL);

		WaitForSingleObject(Threads[0], INFINITE);
		DeleteCriticalSection(&section);
	}
	else {
		TicTacToeBestStep((LPVOID)ThreadParam);
	}
}
void ThreadFunctionForTicTacToeLoselessStep(int **field, int player, int* count_threads, struct Step **answer_buffer, int* size_of_buf) {
	InitializeCriticalSection(&section);
	int*  used_threads = malloc(sizeof(int));
	*used_threads = 0;
	HANDLE* Threads = malloc(sizeof(HANDLE)*(*count_threads));
	struct ThParam*  ThreadParam = malloc(sizeof(struct ThParam));
	ThreadParam->field = field;
	ThreadParam->player = player;
	ThreadParam->count_threads = count_threads;
	ThreadParam->answer_buffer = answer_buffer;
	ThreadParam->size_of_buf = size_of_buf;
	ThreadParam->threads = Threads;
	ThreadParam->used_threads = used_threads;
	if (*count_threads > 0) {

		//EnterCriticalSection(&section);
		*(ThreadParam->used_threads) = *(ThreadParam->used_threads) + 1;
		//LeaveCriticalSection(&section);
		ThreadParam->threads[*(ThreadParam->used_threads) - 1] = CreateThread(NULL, 0, TicTacToeLoselessStepThread, ThreadParam, 0, NULL);

		WaitForSingleObject(Threads[0], INFINITE);
		DeleteCriticalSection(&section);
	}
	else {
		TicTacToeLoselessStep((LPVOID)ThreadParam);
	}
}


void TicTacToeLoselessStep(LPVOID LpParam) {
	struct ThParam *ThreadParam = (struct ThParam*)LpParam;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (ThreadParam->field[i][j] == 0) {
				int** c_field = CopyField(ThreadParam->field);
				c_field[i][j] = ThreadParam->player;
				//printField(c_field);
				if (CheckWinner(c_field) == ThreadParam->player) {
					if (*(ThreadParam->answer_buffer) != NULL)
						*(ThreadParam->answer_buffer) = (struct Step*)realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
					else
						*(ThreadParam->answer_buffer) = (struct Step*)malloc(sizeof(struct Step));
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
					(*(ThreadParam->size_of_buf))++;
				}
				else {
					struct Step* answer_buffer_next = NULL;
					int* abf_size = (int*)malloc(sizeof(int));
					*abf_size = 0;
					struct ThParam * c_LpParam = CopyThreadParam(ThreadParam);
					c_LpParam->field = c_field;
					c_LpParam->player = (ThreadParam->player * 2) % 3;
					c_LpParam->answer_buffer = &answer_buffer_next;
					c_LpParam->size_of_buf = abf_size;
					TicTacToeLoselessStep((LPVOID)c_LpParam);
					if (*abf_size == 0) {
						if (*(ThreadParam->answer_buffer) != NULL)
							*(ThreadParam->answer_buffer) = realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
						else
							*(ThreadParam->answer_buffer) = malloc(sizeof(struct Step));
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
						(*(ThreadParam->size_of_buf))++;
					}
				}
				c_field = DeleteField(c_field);
			}
		}
	}
}


DWORD WINAPI TicTacToeLoselessStepThread(LPVOID LpParam) {
	struct ThParam *ThreadParam = (struct ThParam*)LpParam;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (ThreadParam->field[i][j] == 0) {
				int** c_field = CopyField(ThreadParam->field);
				c_field[i][j] = ThreadParam->player;
				//printField(c_field);
				if (CheckWinner(c_field) == ThreadParam->player) {
					if (*(ThreadParam->answer_buffer) != NULL)
						*(ThreadParam->answer_buffer) = (struct Step*)realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
					else
						*(ThreadParam->answer_buffer) = (struct Step*)malloc(sizeof(struct Step));
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
					(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
					(*(ThreadParam->size_of_buf))++;
				}
				else {
					struct Step* answer_buffer_next = NULL;
					int* abf_size = (int*)malloc(sizeof(int));
					*abf_size = 0;
					struct ThParam * c_LpParam = CopyThreadParam(ThreadParam);
					c_LpParam->field = c_field;
					c_LpParam->player = (ThreadParam->player * 2) % 3;
					//c_LpParam->count_threads = 
					c_LpParam->answer_buffer = &answer_buffer_next;
					c_LpParam->size_of_buf = abf_size;
					int cur_thread = 0;
					if (*(c_LpParam->used_threads) < *(c_LpParam->count_threads)) {
						//printf("%d ", *((*c_LpParam).used_threads));

					//	EnterCriticalSection(&section);
						*(c_LpParam->used_threads) = *(c_LpParam->used_threads) + 1;
					//	LeaveCriticalSection(&section);
						c_LpParam->threads[*(c_LpParam->used_threads) - 1] = CreateThread(NULL, 0, TicTacToeBestStepThread, c_LpParam, 0, NULL);

						cur_thread = *(c_LpParam->used_threads);
					}
					else {
						TicTacToeLoselessStep((LPVOID)c_LpParam);
					}
					if (cur_thread != 0)
						WaitForSingleObject(ThreadParam->threads[cur_thread - 1], INFINITE);
					if (*abf_size == 0) {
						if (*(ThreadParam->answer_buffer) != NULL)
							*(ThreadParam->answer_buffer) = realloc(*(ThreadParam->answer_buffer), (*(ThreadParam->size_of_buf) + 1) * sizeof(struct Step));
						else
							*(ThreadParam->answer_buffer) = malloc(sizeof(struct Step));
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].x = i;
						(*(ThreadParam->answer_buffer))[*(ThreadParam->size_of_buf)].y = j;
						(*(ThreadParam->size_of_buf))++;
					}
				}
				c_field = DeleteField(c_field);
			}
		}
	}
	ExitThread(0);
}

/*
void TicTacToeBestStep(int **field, int player, int* count_threads, struct Step **answer_buffer, int* size_of_buf) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (field[i][j] == 0) {
				int** c_field = CopyField(field);
				c_field[i][j] = player;
				//printField(c_field);
				if (CheckWinner(c_field) == player) {
					if (*answer_buffer != NULL)
						*answer_buffer = (struct Step*)realloc(*answer_buffer, (*(size_of_buf) + 1)*sizeof(struct Step));
					else
						*answer_buffer = (struct Step*)malloc(sizeof(struct Step));
					(*answer_buffer)[*(size_of_buf)].x = i;
					(*answer_buffer)[*(size_of_buf)].y = j;
					(*size_of_buf)++;
				}
				else {
					struct Step* answer_buffer_next = NULL;
					int* abf_size = (int*)malloc(sizeof(int));
					*abf_size = 0;
					TicTacToeBestStep(c_field, (player * 2) % 3, count_threads, &answer_buffer_next, abf_size);
					if (*abf_size == 0 && CheckClearPoint(c_field) > 0) {
						if (*answer_buffer != NULL  )
							*answer_buffer = realloc(*answer_buffer, (*(size_of_buf) + 1)*sizeof(struct Step));
						else
							*answer_buffer = malloc(sizeof(struct Step));
						(*answer_buffer)[*(size_of_buf)].x = i;
						(*answer_buffer)[*(size_of_buf)].y = j;
						(*size_of_buf)++;
					}
				}
				c_field = DeleteField(c_field);
			}
		}
	}
}*/

int CheckClearPoint(int ** field) {
	int res = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (field[i][j] == 0)
				res++;
	return res;
}
struct ThParam* CopyThreadParam(struct ThParam* Param) {
	struct ThParam* Res = malloc(sizeof(struct ThParam));
	Res->field = Param->field;
	Res->player = Param->player;
	Res->count_threads = Param->count_threads;
	Res->answer_buffer = Param->answer_buffer;
	Res->size_of_buf = Param->size_of_buf;
	Res->threads = Param->threads;
	Res->used_threads = Param->used_threads;
	return Res;
}