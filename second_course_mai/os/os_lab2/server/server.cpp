#include<Windows.h>
#include<tchar.h>
#include"CTree.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE inH = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE outH = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD readBytes;
	cTree* ctree = nullptr;
	int a,b,c;
	while (1) {
		ReadFile(inH, &a, sizeof(int), &readBytes, 0);
		switch (a) {
		case 0:
			ReadFile(inH, &b, sizeof(int), &readBytes, 0);
			ctree = cTreeCreate(b);
			break;
		case 1:
			ReadFile(inH, &b, sizeof(int), &readBytes, 0);
			ReadFile(inH, &c, sizeof(int), &readBytes, 0);
			cTreeAddNode(ctree, b, c);
			break;
		case 2:
			ReadFile(inH, &b, sizeof(int), &readBytes, 0);
			cTreeDeleteNode(ctree, b);
			break;
		case 3:
			cTreeDestroy(ctree);
			break;
		case 4:
			cTreePrintTo(ctree, outH);
			break;
		case 5:
			return 0;
		}
	}
	return 0;
}

