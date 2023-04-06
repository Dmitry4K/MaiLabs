#define _CRT_USE_SECURE_NO_WARNINGS
#include <Windows.h>
#include<tchar.h>
#include<iostream>
#include"split.h"
#include<locale.h>

int StrToChar(const char* str) {
	int i = 0, res = 0;
	while (str[i] != '\0') {
		res *= 10;
		res += str[i] - '0';
		i++;
	}
	return res;
}

void State(const char* str) {
	printf(">>> %s\n", str);
}

void Menu() {
	printf("create [ключ]     - создать дерево\n");
	printf("add [ключ] [ключ] - добавить узел\n");
	printf("del [ключ]        - удалить узел\n");
	printf("clear             - очистить дерево\n");
	printf("print             - распечатать дерево\n");
	printf("exit              - выйти\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));//обнулить ProcessInfo
	STARTUPINFO StartupInfo; //This is an [in] parameter
	
	TCHAR lpszClientPath[] = "server";//название процесса
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field
	HANDLE pipe1Read, pipe1Write, pipe2Read, pipe2Write;//идентификатор объекта
	CreatePipe(&pipe1Read, &pipe1Write, &sa, 0);

	CreatePipe(&pipe2Read, &pipe2Write, &sa, 0);
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;
	StartupInfo.hStdInput = pipe1Read;
	StartupInfo.hStdOutput = pipe2Write;

	bool process = CreateProcess(NULL,
		lpszClientPath,//процесс
		NULL, NULL, true,
		CREATE_NO_WINDOW,// CREATE_NEW_CONSOLE|CREATE_SUSPENDED
		NULL, NULL,
		&StartupInfo,
		&ProcessInfo);
	process ? State("процесс создан") : State("ошибка: процесс не создан");
	CloseHandle(pipe1Read);
	CloseHandle(pipe2Write);

	DWORD writeBytes, readBytes;

	char masstr[256];
	char *str;

	printf("\n");

	Menu();
	printf("\n");
	while (1) {
		str = gets_s(masstr, 256);
		char **commands = split(str, ' ');
		bool isSuccess;
		int i = 0, num;
		while (commands[i][0] != '\0') {
			if (!strcmp(commands[i], "create")) {
				num = 0;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено create") : State("Команда не отправлена");
				i++;
				num = StrToChar(commands[i]);
				WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
			}
			else if (!strcmp(commands[i], "add")) {
				num = 1;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено add") : State("Команда не отправлена");
				i++;
				num = StrToChar(commands[i]);
				WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				i++;
				num = StrToChar(commands[i]);
				WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
			}
			else if (!strcmp(commands[i], "del")) {
				num = 2;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено del") : State("Команда не отправлена");
				i++;
				num = StrToChar(commands[i]);
				WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
			}
			else if (!strcmp(commands[i], "clear")) {
				num = 3;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено clear") : State("Команда не отправлена");
			}
			else if (!strcmp(commands[i], "print")) {
				num = 4;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено print") : State("Команда не отправлена");
				char c = '0';
				while (c != '\0') {
					ReadFile(pipe2Read, &c, sizeof(char), &readBytes, NULL);
					printf("%c", c);
				}
				printf("\b");
			}
			else if (!strcmp(commands[i], "exit")) {
				num = 5;
				isSuccess = WriteFile(pipe1Write, &num, sizeof(int), &writeBytes, NULL);
				isSuccess ? State("отправлено exit") : State("Команда не отправлена");
				CloseHandle(pipe2Read);
				CloseHandle(pipe1Write);
				CloseHandle(ProcessInfo.hThread);
				CloseHandle(ProcessInfo.hProcess);
				system("pause");
				return 0;
			}
			i++;
		}
	}


	CloseHandle(pipe2Read);
	CloseHandle(pipe1Write);
	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);
	system("pause");
	return 0;
}