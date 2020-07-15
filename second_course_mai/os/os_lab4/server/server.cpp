#define MAP_LOCATION L"Local\\FileMapObject"
#define EVENT_NAME_FIRST L"Local\\FirstEvent"
#define EVENT_NAME_SECOND L"Local\\SecondEvent"
#define MUTEX_NAME L"Local\\newmutex"
#include<Windows.h>
#include<tchar.h>
#include"CTree.h"
#include<iostream>
#include"split.h"
int StrToChar(const char* str) {
	int i = 0, res = 0;
	while (str[i] != '\0') {
		res *= 10;
		res += str[i] - '0';
		i++;
	}
	return res;
}
int _tmain(int argc, _TCHAR* argv[])
{
	/*
	HANDLE hMutex = OpenMutex(
		MUTEX_ALL_ACCESS,                        // default security descriptor
		FALSE,                       // mutex not owned
		MUTEX_NAME);
	
	if (hMutex == 0) {
		std::cout << "Mutex Error\n"<<GetLastError();
	}	*/

	HANDLE hEvent1 = OpenEvent(EVENT_ALL_ACCESS,FALSE, EVENT_NAME_FIRST);
	HANDLE hEvent2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME_SECOND);
	if (hEvent1 == 0) {
		std::cout << "Event error: " << GetLastError() << std::endl;
		system("pause");
		return 1;
	}
	if (hEvent2 == 0) {
		std::cout << "Event error: " << GetLastError() << std::endl;	
		system("pause");
		return 1;
	}
	cTree* ctree = nullptr;
	HANDLE hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, MAP_LOCATION);
	if (hFileMap == INVALID_HANDLE_VALUE) {
		std::cout << "Error filemapping\n";
	}
	while (1) {
		//WaitForSingleObject(hMutex, INFINITE);
		WaitForSingleObject(hEvent2, INFINITE);
		//Sleep(2000);
		PCHAR lbBuffer = (PCHAR)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 256);
		if (lbBuffer == nullptr) {
			std::cout << "Error reading map\n";
		}
		char** commands = split((char*)lbBuffer, ' ');
		bool NeedClear = true;
		int i = 0, a,b,c;
		std::string buffer;
		while (commands[i][0] != '\0') {
			if (!strcmp(commands[i], "create")) {
				std::cout << "create\n";
				i++;
				a = StrToChar(commands[i]);
				ctree = cTreeCreate(a);
			}
			else if (!strcmp(commands[i], "add")) {
				std::cout << "add\n";
				i++;
				b = StrToChar(commands[i]);
				i++;
				c = StrToChar(commands[i]);
				cTreeAddNode(ctree, b, c);
			}
			else if (!strcmp(commands[i], "del")) {
				std::cout << "del\n";
				i++;
				b = StrToChar(commands[i]);
				cTreeDeleteNode(ctree, b);
			}
			else if (!strcmp(commands[i], "clear")) {
				std::cout << "clear\n";
				cTreeDestroy(ctree);
			}
			else if (!strcmp(commands[i], "print")) {
				ZeroMemory(lbBuffer, sizeof(PCHAR));
				NeedClear = false;
				std::cout << "print\n";
				buffer = cTreePrintToPtr(ctree);
				std::cout << buffer;
				CopyMemory(lbBuffer, buffer.data(), 256);
				//printf("\b");
			}
			else if (!strcmp(commands[i], "exit")) {
				ZeroMemory(lbBuffer, sizeof(PCHAR));
				//(hMutex);
				SetEvent(hEvent1);
				return 0;
			}
			i++;
		}
		if(NeedClear)
			ZeroMemory(lbBuffer, sizeof(PCHAR));
		ResetEvent(hEvent2);
		SetEvent(hEvent1);
		//ReleaseMutex(hMutex);
	}
	system("pause");
	return 0;
}

