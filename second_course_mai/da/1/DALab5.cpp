#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<set>
#include<limits>
#include<map>
#include<algorithm>

const char TERMINATE_SYMBOL = '$';
																		//узел суффиксного дерева
struct TSuffixTreeNode {
	std::pair<std::string::iterator, std::string::iterator> subString;								//пара - итераторы на начало и конец подстроки
	std::map<char, TSuffixTreeNode*> nodes;															//дочерние узлы (map - для быстрого перехода по ним)
	TSuffixTreeNode* suffixRef = nullptr;															//суффиксная ссылка
	std::string* str;																				//указатель на строку, которую поддерживает узел и дерево
	bool isBlank = false;
	bool isRoot = false;
	TSuffixTreeNode(std::string* str);																//конструктор для blank
	TSuffixTreeNode(std::string* str, std::string::iterator first, std::string::iterator second);	//конструктор для обычного узла
	~TSuffixTreeNode();
	int Length();																					//возвращает длину текущего ребра
	int Length(std::string::iterator position);														//возвращает длину текущего ребра до заданной позиции
	void BuildArrayDFS(std::vector<int>& res, int len);												//дочерняя функция для потсроения суффиксного массива
};
struct TSuffixTree {
	TSuffixTreeNode* root;
	TSuffixTreeNode* blank;
	std::string str;
	std::vector<int> suffixArray;																	//суффиксный массив
	TSuffixTree(std::string newStr);
	void BuildTree();																				//алгоритм Укконена
	void BuildArray();																				//вычисляет суффиксный массив
	std::vector<int> Search(std::string& p);														//поиск подстрок p в дереве
	~TSuffixTree();
};
																//класс активного узла (для алгоритма Укконена)
struct TSuffixTreeActivePoint {																		
	TSuffixTreeNode* activePoint;																						//указатель на активный узел
	TSuffixTreeNode* makeSR;																								//указатель на узел, которому нужно добавить суф. ссылку
	TSuffixTreeNode* root;
	TSuffixTreeNode* blank;

	int len = 0;																										//активная длина
	int rem = 0;																										//остаток
	std::string::iterator edge;																							//активный символ
	std::string* str;
	TSuffixTreeActivePoint(TSuffixTreeNode* startPoint, TSuffixTreeNode* root, TSuffixTreeNode* blank, std::string* s);
	void goTo(TSuffixTreeNode* node, std::string::iterator c);															//спускаемся вниз по узлу node, по символу с
	TSuffixTreeNode* getPoint();																							//указатель на активный узел
	void addSufRef(TSuffixTreeNode* node);																				//добавить суффиксную ссылку в node
};


TSuffixTree::TSuffixTree(std::string newStr) {
	str = newStr + TERMINATE_SYMBOL;
	blank = new TSuffixTreeNode(&str);
	blank->isBlank = true;
	root = new TSuffixTreeNode(&str);
	for (char i = 'a'; i <= 'z'; ++i) {
		blank->nodes[i] = root;
	}
	blank->nodes[TERMINATE_SYMBOL] = root;
	root->suffixRef = blank;
	BuildTree();
}

void TSuffixTree::BuildTree() {
	TSuffixTreeActivePoint activePoint(root, root, blank, &str);
	std::string::iterator it = str.begin();
	while (it != str.end()) {
		activePoint.makeSR = root;
		++activePoint.rem;
		while (activePoint.rem > 0) {
			if (activePoint.len == 0) {
				activePoint.edge = it;
			}
			std::map<char, TSuffixTreeNode*>::iterator next = activePoint.getPoint()->nodes.find(*activePoint.edge);
			if (next == activePoint.getPoint()->nodes.end()) {
				activePoint.getPoint()->nodes[*activePoint.edge] = new TSuffixTreeNode(&str, it, str.end());
				activePoint.addSufRef(activePoint.getPoint());
			}
			else {

				if (activePoint.len >= next->second->Length(it)) {
					activePoint.goTo(next->second, it);
					continue;
				}

				if (*(next->second->subString.first + activePoint.len) == *it) {
					++activePoint.len;
					activePoint.addSufRef(activePoint.getPoint());
					break;
				}
				TSuffixTreeNode* split = new TSuffixTreeNode(&str, next->second->subString.first, next->second->subString.first + activePoint.len);
				TSuffixTreeNode* newNode = new TSuffixTreeNode(&str, it, str.end());
				TSuffixTreeNode* nextPtr = next->second;
				activePoint.getPoint()->nodes[*activePoint.edge] = split;
				split->nodes[*it] = newNode;
				nextPtr->subString.first += activePoint.len;
				split->nodes[*nextPtr->subString.first] = nextPtr;
				activePoint.addSufRef(split);
			}
			--activePoint.rem;
			if (activePoint.getPoint() == root && activePoint.len > 0) {//если AP - root или len > 0
				--activePoint.len;//уменьшаем активную длину
				activePoint.edge = it - activePoint.rem + 1; //переходим ?
			}
			else {
				activePoint.activePoint = (activePoint.getPoint()->suffixRef) ? activePoint.getPoint()->suffixRef : root; //если не в root или len = 0, то переходим по суф. ссылку, или в корень
			}
		}
		it++;
	}
}
TSuffixTreeNode* TSuffixTreeActivePoint::getPoint() {
	return activePoint;
}
TSuffixTreeActivePoint::TSuffixTreeActivePoint(TSuffixTreeNode* startPoint, TSuffixTreeNode* r, TSuffixTreeNode* b, std::string* s) : activePoint(startPoint), root(r), blank(b), str(s) {}

void TSuffixTreeActivePoint::goTo(TSuffixTreeNode* node, std::string::iterator c) {
	edge += node->Length(c);
	len -= node->Length(c);
	activePoint = node;
}

int TSuffixTreeNode::Length() {
	if (isRoot) {
		return 0;
	}
	return subString.second - subString.first + 1;
}
int TSuffixTreeNode::Length(std::string::iterator position) {
	if (isRoot) {
		return 0;
	}
	return min(subString.second, position + 1) - subString.first;
}
void TSuffixTreeActivePoint::addSufRef(TSuffixTreeNode* node) {
	if (makeSR != root) {
		makeSR->suffixRef = node;
	}
	makeSR = node;
}
TSuffixTreeNode::TSuffixTreeNode(std::string* s) {
	str = s;
	isRoot = true;
}
TSuffixTreeNode::TSuffixTreeNode(std::string* s, std::string::iterator first, std::string::iterator second) {
	str = s;
	subString = { first, second };
}

void TSuffixTree::BuildArray() {
	root->BuildArrayDFS(suffixArray, 0);
}
void TSuffixTreeNode::BuildArrayDFS(std::vector<int>& res, int len) {
	if (nodes.empty()) {
		res.push_back(str->length() - len);
		return;
	}
	for (std::map<char, TSuffixTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		it->second->BuildArrayDFS(res, len + it->second->Length() - 1);
	}
}
TSuffixTreeNode::~TSuffixTreeNode() {
	if (isBlank) {
		nodes.clear();
	}
	else {
		for (std::map<char, TSuffixTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			delete it->second;
			//it->second = nullptr;
		}
		suffixRef = nullptr;
		str = nullptr;
	}
}
TSuffixTree::~TSuffixTree() {

	if (root) {
		delete root;
	}
	if (blank) {
		delete blank;
		blank = nullptr;
	}
}


int LowerBound(int left, int right, int i, std::string& text, std::string& p, std::vector<int> suffixArray) {
	bool found = false;
	int middle;
	while ((left <= right) && (found != true)) {
		middle = (left + right) / 2;
		if (text[suffixArray[middle]+i] == p[i] ) {
			if (middle > left) {
				if (text[suffixArray[middle] + i] == (suffixArray[middle - 1] + i < text.size() ? text[suffixArray[middle - 1] + i] : '$')) {
					right = middle - 1;
				}
				else {
					found = true;
				}
			}
			else {
				found = true;//?
			}
		}
		else if (text[suffixArray[middle]+i] > p[i]) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}

	if (found) return middle;
	else return -1;
}
int UpperBound(int left, int right, int i, std::string& text, std::string& p, std::vector<int> suffixArray) {
	bool found = false;
	int middle;
	while ((left <= right) && (found != true)) {
		middle = (left + right) / 2;
		if (text[suffixArray[middle] + i] == p[i]) {
			if (middle <  right) {
				if (text[suffixArray[middle] + i] == (suffixArray[middle + 1] + i < text.size() ? text[suffixArray[middle + 1] + i] : '$')) {
					left = middle + 1;
				}
				else {
					found = true;
				}
			}
			else {
				found = true;//?
			}
		}
		else if (text[suffixArray[middle] + i] > p[i]) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}

	if (found) return middle;
	else return -1;
}

template<class T>
void countingSort(std::vector<T>& a) {
	T maxEl = a[0];
	for (int i = 1; i < a.size(); ++i) {
		if (maxEl < a[i]) {
			maxEl = a[i];
		}
	}
	std::vector<T> c(maxEl + 1);
	for (int i = 0; i <= maxEl; ++i)
		c[i] = 0;
	for (int i = 0; i < a.size(); ++i)
		c[a[i]]++;
	for (int i = 1; i <= maxEl; ++i)
		c[i] += c[i - 1];
	std::vector<T> b(a.size());
	for (int i = a.size() - 1; i >= 0; i--) {
		c[a[i]]--;
		b[c[a[i]]] = a[i];
	}
	for (int i = 0; i < a.size(); i++)
		a[i] = b[i];
}

std::vector<int> TSuffixTree::Search(std::string& p) {
	std::vector<int> res;
	if (p.length() >= str.length()) {
		return res;
	}
	int left = 1;                                     
	int	right = suffixArray.size()-1;                       
	for (int i = 0; i < p.length(); ++i) {
		left = LowerBound(left, right, i, str, p, suffixArray);
		if (left == -1) {
			return res;
		}
		right = UpperBound(left, right,  i, str, p, suffixArray);
		if (right == -1) {
			return res;
		}
	}
	if (right - left < 0)  {
		return res;
	}
	res = std::vector<int>(right - left + 1);
	for (int i = 0; left <= right; ++left, ++i) {
		res[i] = suffixArray[left] + 1;
	}
	countingSort<int>(res);
	return res;
}


int main() {
	std::string line, str;
	std::cin >> line;
	TSuffixTree myTree(line);
	std::vector<int> res;
	myTree.BuildArray();
	int i = 1;
	while (std::cin>>str) {
		res = myTree.Search(str);
		if (!res.empty()) {
			std::cout << i << ": " << res[0];
			for (int i = 1; i < res.size(); ++i) {
				std::cout << ", " << res[i];
			}
			std::cout << std::endl;
		}
		++i;
	}
}
