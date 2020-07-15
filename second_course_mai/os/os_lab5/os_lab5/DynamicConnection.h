#pragma once
struct cNode {
	int key;
	cNode* parent;
	cNode* son;
	cNode* brother;
};

struct cTree {
	cNode* root;
};