// MyDLL.cpp : Определяет экспортируемые функции для DLL.
//

#include "pch.h"
#include "framework.h"
#include "MyDLL.h"

#include<malloc.h>
#include<iostream>
#include<string>
/*
// Пример экспортированной переменной
MYDLL_API int nMyDLL=0;

// Пример экспортированной функции.
MYDLL_API int fnMyDLL(void)
{
    return 0;
}

// Конструктор для экспортированного класса.
CMyDLL::CMyDLL()
{
    return;
}*/
MYDLL_API cTree* cTreeCreate(int key) {
	cTree* res = (cTree*)malloc(sizeof(cTree));
	res->root = (cNode*)malloc(sizeof(cNode));
	res->root->key = key;
	res->root->parent = nullptr;
	res->root->brother = nullptr;
	res->root->son = nullptr;
	return res;
}

MYDLL_API cNode* cTreeFindNodeByKey(cNode* node, int key) {
	cNode* res = nullptr;
	if (node->key == key)
		return node;
	if (node->son)
		res = cTreeFindNodeByKey(node->son, key);
	if (node->brother && !res)
		res = cTreeFindNodeByKey(node->brother, key);
	return res;
}

MYDLL_API void cTreeAddNode(cTree* tree, int to, int key) {
	cNode* fnode = nullptr;
	fnode = cTreeFindNodeByKey(tree->root, to);
	if (!fnode) {
		return;
	}
	if (!fnode->son) {
		fnode->son = (cNode*)malloc(sizeof(cNode));
		fnode->son->key = key;
		fnode->son->parent = fnode;
		fnode->son->brother = nullptr;
		fnode->son->son = nullptr;
		return;
	}
	else {
		cNode* bnode = nullptr;
		bnode = fnode->son;
		while (bnode->brother)
			bnode = bnode->brother;
		bnode->brother = (cNode*)malloc(sizeof(cNode));
		bnode->brother->key = key;
		bnode->brother->parent = fnode;
		bnode->brother->brother = nullptr;
		bnode->brother->son = nullptr;
		return;
	}
	return;
}

MYDLL_API void cTreeDeleteNode(cTree* tree, int key) {
	cNode* fnode = nullptr;
	fnode = cTreeFindNodeByKey(tree->root, key);
	if (!fnode)
		return;

	if (fnode->son)
		cTreeClear(fnode->son);

	cNode* inode = fnode->parent->son;
	if (inode == fnode) {
		if (fnode->brother)
			fnode->parent->son = fnode->brother;
		else
			fnode->parent->son = nullptr;
		free(fnode);
	}
	else {
		while (inode->brother != fnode) {
			inode = inode->brother;
		}
		if (fnode->brother) {
			inode->brother = fnode->brother;
			free(fnode);
		}
		else {
			inode->brother = nullptr;
			free(fnode);
		}
	}
}
MYDLL_API void cTreeClear(cNode* node) {
	if (node->son)
		cTreeClear(node->son);
	if (node->brother)
		cTreeClear(node->brother);
	node->brother = nullptr;
	node->son = nullptr;
	if (node->parent)
		if (node->parent->son == node)
			node->parent->son = nullptr;
	free(node);
}

MYDLL_API void cTreeDestroy(cTree* tree) {
	cTreeClear(tree->root);
	tree->root = nullptr;
}

MYDLL_API void cTreePrint(cTree* tree) {
	if (tree->root)
		cTreePrint_(tree->root, 0);
}

MYDLL_API void cTreePrint_(cNode* node, int count) {
	for (int i = 0; i < count; i++)
		printf("\t");
	printf("%d\n", node->key);
	if (node->son)
		cTreePrint_(node->son, count + 1);
	if (node->brother)
		cTreePrint_(node->brother, count);
}


MYDLL_API void cTreePrintTo(cTree* tree, HANDLE outH) {
	DWORD writebytes = 0;
	char c = '\0';
	if (tree->root)
		cTreePrintTo_(tree->root, 0, outH, &writebytes);
	WriteFile(outH, &c, sizeof(char), &writebytes, 0);
}

MYDLL_API void cTreePrintTo_(cNode* node, int count, HANDLE outH, DWORD* writebytes) {
	char c = '\t';
	for (int i = 0; i < count; i++)
		WriteFile(outH, &c, sizeof(char), writebytes, 0);
	c = node->key + '0';
	WriteFile(outH, &c, sizeof(char), writebytes, 0);
	c = '\n';
	WriteFile(outH, &c, sizeof(char), writebytes, 0);
	if (node->son)
		cTreePrintTo_(node->son, count + 1, outH, writebytes);
	if (node->brother)
		cTreePrintTo_(node->brother, count, outH, writebytes);
}
MYDLL_API void cTreePrintToPtr_(cNode* node, int count, std::string* res) {
	char c = '\t';
	for (int i = 0; i < count; i++)
		(*res) += c;
	//WriteFile(outH, &c, sizeof(char), writebytes, 0);
	c = node->key + '0';
	(*res) += c;
	//WriteFile(outH, &c, sizeof(char), writebytes, 0);
	c = '\n';
	(*res) += c;
	//WriteFile(outH, &c, sizeof(char), writebytes, 0);
	if (node->son)
		cTreePrintToPtr_(node->son, count + 1, res);
	if (node->brother)
		cTreePrintToPtr_(node->brother, count, res);
}
MYDLL_API std::string cTreePrintToPtr(cTree* tree) {
	//char c = '\0';
	std::string res;
	if (tree->root)
		cTreePrintToPtr_(tree->root, 0, &res);
	res += '\0';
	return res;
	//WriteFile(outH, &c, sizeof(char), &writebytes, 0);
}