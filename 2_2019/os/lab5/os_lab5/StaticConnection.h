#pragma once
#define MYDLL_API __declspec(dllimport)
#include<Windows.h>
#include<string>
extern  struct cNode {
int key;
cNode* parent;
cNode* son;
cNode* brother;
};

extern  struct cTree {
	cNode* root;
};
MYDLL_API cTree* cTreeCreate(int key);//проверено
MYDLL_API cNode* cTreeFindNodeByKey(cNode* tree, int key);//проверено
MYDLL_API void cTreeAddNode(cTree* tree, int to, int key);//проверено
MYDLL_API void cTreeDeleteNode(cTree* tree, int key);
MYDLL_API void cTreeClear(cNode* node);//проверено
MYDLL_API void cTreeDestroy(cTree* tree);//проверено
MYDLL_API void cTreePrint(cTree* tree);
MYDLL_API void cTreePrint_(cNode* node, int count);
MYDLL_API void cTreePrintTo(cTree* tree, HANDLE outH);
MYDLL_API void cTreePrintTo_(cNode* node, int count, HANDLE outH, DWORD* writebytes);
MYDLL_API std::string cTreePrintToPtr(cTree* tree);
MYDLL_API void cTreePrintToPtr_(cNode* node, int count, std::string* res);
