#define _CRT_USE_SECURE_NO_WARNINGS
#define EVENT_NAME_FIRST L"Local\\FirstEvent"
#define EVENT_NAME_SECOND L"Local\\SecondEvent"
#define MAP_LOCATION L"Local\\FileMapObject"
#define MUTEX_NAME L"Local\\newmutex"
#include <Windows.h>
#include<tchar.h>
#include<iostream>
#include"split.h"
#include<locale.h>
#include<string>

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
	TCHAR lpszClientPath[] = L"server";//название процесса
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO); //Only compulsory field

	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 256, MAP_LOCATION);
	if (hFileMap == INVALID_HANDLE_VALUE) {
		State("Error: file not mapped\n");
		system("pause");
		return 0;
	}

	PCHAR lbBuffer = (PCHAR)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 256);
	if (lbBuffer == nullptr) {
		State("Error reading map");
		system("pause");
		return 0;
	}
	HANDLE hEvent1 = CreateEvent(NULL, FALSE, FALSE, EVENT_NAME_FIRST);
	HANDLE hEvent2 = CreateEvent(NULL, FALSE, FALSE, EVENT_NAME_SECOND);
/*
	HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);//залоченый мютекс
	if (hMutex == 0) {
		std::cout << "Creating Mutex Error : " << GetLastError()<<std::endl;
	}
	*/
	bool process = CreateProcess(NULL,lpszClientPath,NULL, NULL, TRUE,CREATE_NEW_CONSOLE,NULL, NULL,&StartupInfo,&ProcessInfo);
	process ? State("процесс создан") : State("ошибка: процесс не создан");
	DWORD writeBytes, readBytes;

	//ar masstr[256];
	//char *str = nullptr;
	printf("\n");

	Menu();
	printf("\n");
	std::string line;
	bool isExit = false;
	int i = 0;
	while (true){
		getline(std::cin, line);
		char** comands = split(const_cast<char*>(line.data()), ' ');
		while (comands[i][0] != '\0') {
			if (!strcmp(comands[i++], "exit"))
				isExit = true;
		}
		i = 0;
		CopyMemory(lbBuffer, line.data(), 256);
		//ReleaseMutex(hMutex);
		ResetEvent(hEvent1);
		SetEvent(hEvent2);
		WaitForSingleObject(hEvent1,INFINITE);
		std::cout << lbBuffer;
		if (isExit)
			break;
	}
	//UnmapViewOfFile(MAP_LOCATION);
	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(hEvent1);
	CloseHandle(hEvent2);
	return 0;
}