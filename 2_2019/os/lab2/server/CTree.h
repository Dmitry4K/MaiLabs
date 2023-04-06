#pragma once
#include<Windows.h>
struct cNode {
	int key;
	cNode* parent;
	cNode* son;
	cNode* brother;
};

struct cTree {
	cNode* root;
};


cTree* cTreeCreate(int key);//���������
cNode* cTreeFindNodeByKey(cNode* tree, int key);//���������
void cTreeAddNode(cTree* tree, int to, int key);//���������
void cTreeDeleteNode(cTree* tree, int key);
void cTreeClear(cNode* node);//���������
void cTreeDestroy(cTree* tree);//���������
void cTreePrint(cTree* tree);
void cTreePrint_(cNode* node, int count);
void cTreePrintTo(cTree* tree, HANDLE outH);
void cTreePrintTo_(cNode* node, int count, HANDLE outH, DWORD* writebytes);