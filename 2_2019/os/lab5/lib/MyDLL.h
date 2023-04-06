// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа MYDLL_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции MYDLL_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif


#include<string>
// Этот класс экспортирован из библиотеки DLL
/*
class MYDLL_API CMyDLL {
public:
	CMyDLL(void);
	// TODO: добавьте сюда свои методы.
};

extern MYDLL_API int nMyDLL;

MYDLL_API int fnMyDLL(void);
*/
extern MYDLL_API struct cNode {
	int key;
	cNode* parent;
	cNode* son;
	cNode* brother;
};

extern MYDLL_API struct cTree {
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
