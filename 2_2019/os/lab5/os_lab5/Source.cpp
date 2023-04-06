#define LIB_LOCATION L"MYDLL.dll"
#include<Windows.h>
#include<iostream>
//#include"DynamicConnection.h"
#include"StaticConnection.h"
int main() {
	/*
	HMODULE hDLL = LoadLibrary(LIB_LOCATION);
	if (!hDLL) {
		std::cout << "Error, DLL not found\n";
		return 1;
	}
	cTree* (*cTreeCreate)(int);
	(FARPROC&)cTreeCreate = GetProcAddress(hDLL, "cTreeCreate");
	if (cTreeCreate == NULL) {
		std::cout << "error\n"<<GetLastError();
	}
	void (*cTreePrint)(cTree*);
	(FARPROC&)cTreePrint = GetProcAddress(hDLL, "cTreePrint");
	void (*cTreePrint_)(cTree*,int);
	(FARPROC&)cTreePrint_ = GetProcAddress(hDLL, "cTreePrint_");
	cNode* (*cTreeFindNodeByKey)(cNode, int);
	(FARPROC&)cTreeFindNodeByKey = GetProcAddress(hDLL, "cTreeFindNodeByKey"); 
	void (*cTreeAddNode)(cTree* , int , int );//проверено
	(FARPROC&)cTreeAddNode = GetProcAddress(hDLL, "cTreeAddNode"); 
	void (*cTreeDeleteNode)(cTree * , int );
	(FARPROC&)cTreeDeleteNode = GetProcAddress(hDLL, "cTreeDeleteNode"); 
	void (*cTreeClear)(cNode * );//проверено
	(FARPROC&)cTreeClear = GetProcAddress(hDLL, "cTreeClear"); 
	void (*cTreeDestroy)(cTree * );//проверено
	(FARPROC&)cTreeDestroy = GetProcAddress(hDLL, "cTreeDestroy"); 
	void (*cTreePrintTo)(cTree * , HANDLE );
	(FARPROC&)cTreePrintTo = GetProcAddress(hDLL, "cTreePrintTo"); 
	void (*cTreePrintTo_)(cNode * , int , HANDLE , DWORD * );
	(FARPROC&)cTreePrintTo_ = GetProcAddress(hDLL, "cTreePrintTo_"); 
	std::string (*cTreePrintToPtr)(cTree*);
	(FARPROC&)cTreePrintToPtr = GetProcAddress(hDLL, "cTreePrintToPtr"); 
	void (*cTreePrintToPtr_)(cNode * , int , std::string * );
	(FARPROC&)cTreePrintToPtr_ = GetProcAddress(hDLL, "cTreePrintToPtr_"); 
	//*/

	cTree* ctree = nullptr;
	ctree = cTreeCreate(0);
	cTreeAddNode(ctree, 0, 1);
	cTreeAddNode(ctree, 0, 1); 
	cTreeAddNode(ctree, 1, 2);
	cTreeAddNode(ctree, 2, 3);
	cTreeDeleteNode(ctree, 3);
	cTreePrint(ctree);
	cTreeDestroy(ctree);
	cTreePrint(ctree);
	
	system("pause");
	return 0;
}