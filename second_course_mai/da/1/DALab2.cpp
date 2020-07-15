//#include<vld.h>
#include<iostream>
#include<fstream>
#include<cctype>
#include<string.h>

const int LINE_SIZE = 264;

template<typename T, int P> class TBTree {//T - тип ключа, P - ’арактеристика дерева
	template<typename, int> struct TBTreeNode { //узел B-ƒерева
		int N; //n - кол-во ключей в узле
		T* Keys; //Keys - n-ключей
		TBTreeNode<T, P>** Nodes; //Nodes - массив указателй на след узлы
		bool Leaf; //Leaf - харатеристика узла, лист или не лист
		TBTreeNode() : N(0), Leaf(true) { // по умолчанию в узле ничего не хранитс¤ и узел ¤вл¤етс¤ листом
			Keys = new T[2 * P - 1];
			Nodes = new TBTreeNode<T, P> * [2 * P];
			for (int i = 0; i < 2 * P; ++i)
				Nodes[i] = nullptr;
		}
		~TBTreeNode() { // удал¤ютс¤ все ключи в узле и ссылки на другие, при этом дочерние остаютс¤ в пам¤ти
			if (Keys) {
				delete[] Keys;
				Keys = nullptr;
			}
			if (Nodes) {
				delete[] Nodes;
				Nodes = nullptr;
			}
			N= 0;
		}
		T* BTreeSearch(T k) { //поиск ключа в дереве
			if (this) {
				int i = 0;
				while ((i < N) && (k > Keys[i])) {
					++i;
				}
				if ((i < N) && k == Keys[i]) {
					return (Keys + i);
				}
				if (Leaf) {
					return nullptr;
				}
				else {
					return Nodes[i]->BTreeSearch(k);
				}
			}
			else
				return nullptr;
		}
		void BTreeSplitChild(TBTreeNode<T, P>* y, int i) { //разделение дочерненго узла по индексу
			TBTreeNode<T, P>* z = new TBTreeNode<T, P>();
			z->Leaf = y->Leaf;
			z->N= P - 1;
			for (int j = 0; j < P - 1; ++j) {
				z->Keys[j] = y->Keys[j + P];
			}
			if (!(y->Leaf)) {
				for (int j = 0; j < P; ++j) {
					z->Nodes[j] = y->Nodes[j + P];
				}
			}
			y->N= P - 1;
			for (int j = N; j >= i + 1; --j) {
				Nodes[j + 1] = Nodes[j];
			}
			Nodes[i + 1] = z;
			for (int j =N- 1; j >= i; --j) {
				Keys[j + 1] = Keys[j];
			}
			Keys[i] = y->Keys[P - 1];
			++N;
		}
		void BTreeInsertNonfull(T k) {//вставка в ключа в узел
			int i =N- 1;
			if (Leaf) {
				while ((i >= 0) && (k < Keys[i])) {
					Keys[i + 1] = Keys[i];
					--i;
				}
				Keys[i + 1] = k;
				++N;
			}
			else {
				while ((i >= 0) && (k < Keys[i])) {
					--i;
				}
				++i;
				if (Nodes[i]->N== (2 * P - 1)) {
					BTreeSplitChild(Nodes[i], i);
					if (k > Keys[i]) {
						++i;
					}
				}
				Nodes[i]->BTreeInsertNonfull(k);
			}
		}
		int BTreeFindKey(T k) {//поиск ключа в текущем узле.
			int index = 0;
			while (index <N&& Keys[index] < k) {
				++index;
			}
			return index;
		}
		void BTreeDeleteFromLeaf(int index) {//удаление ключа из узла, который ¤вл¤етс¤ листом
			for (int i = index + 1; i < N; ++i) {
				Keys[i - 1] = Keys[i];
			}
			--N;
		}
		void BTreeDeleteFromNonLeaf(int index) {//удаление ключа из узла, который ¤вл¤етс¤ листом
			T k = Keys[index];
			if (Nodes[index]->N>= P) {
				T pred = BTreeNodeGetPred(index);
				Keys[index] = pred;
				Nodes[index]->BTreeDeleteNode(pred);
			}
			else if (Nodes[index + 1]->N >= P) {
				T succ = BTreeNodeGetSucc(index);
				Keys[index] = succ;
				Nodes[index + 1]->BTreeDeleteNode(succ);
			}
			else {
				BTreeMerge(index);
				Nodes[index]->BTreeDeleteNode(k);
			}
		}
		T BTreeNodeGetPred(int index) { //достать у узла с индексом index - самый близкий слева узел хран¤щийс¤ в дереве 
			TBTreeNode<T, P>* current = Nodes[index];
			while (!current->Leaf) {
				current = current->Nodes[current->N];
			}
			return current->Keys[current->N- 1];
		}
		T BTreeNodeGetSucc(int index) {//достать у узла с индексом index - самый близкий справа узел хран¤щийс¤ в дереве 
			TBTreeNode<T, P>* current = Nodes[index + 1];
			while (!current->Leaf) {
				current = current->Nodes[0];
			}
			return current->Keys[0];
		}
		void BTreeFill(int index) { 
			if (index != 0 && Nodes[index - 1]->N>= P) {
				BTreeBorrowFromPrev(index);
			}
			else if (index !=N&& Nodes[index + 1]->N>= P) {
				BTreeBorrowFromNext(index);
			}
			else {
				if (index != N) {
					BTreeMerge(index);
				}
				else {
					BTreeMerge(index - 1);
				}
			}
		}
		void BTreeBorrowFromPrev(int index) {
			TBTreeNode<T, P>* child = Nodes[index];
			TBTreeNode<T, P>* sibling = Nodes[index - 1];
			for (int i = child->N- 1; i >= 0; --i) {
				child->Keys[i + 1] = child->Keys[i];
			}
			if (!child->Leaf) {
				for (int i = child->N; i >= 0; --i) {
					child->Nodes[i + 1] = child->Nodes[i];
				}
			}
			child->Keys[0] = Keys[index - 1];
			if (!child->Leaf) {
				child->Nodes[0] = sibling->Nodes[sibling->N];
			}
			Keys[index - 1] = sibling->Keys[sibling->N- 1];
			child->N+= 1;
			sibling->N-= 1;
		}
		void BTreeBorrowFromNext(int index) {
			TBTreeNode<T, P>* child = Nodes[index];
			TBTreeNode<T, P>* sibling = Nodes[index + 1];
			child->Keys[child->N] = Keys[index];
			if (!child->Leaf) {
				child->Nodes[child->N+ 1] = sibling->Nodes[0];
			}
			Keys[index] = sibling->Keys[0];
			for (int i = 1; i < sibling->N; ++i) {
				sibling->Keys[i - 1] = sibling->Keys[i];
			}
			if (!sibling->Leaf) {
				for (int i = 1; i <= sibling->N; ++i) {
					sibling->Nodes[i - 1] = sibling->Nodes[i];
				}
			}
			child->N+= 1;
			sibling->N-= 1;
		}
		void BTreeMerge(int index) { //объединить узел index и index+1
			TBTreeNode<T, P>* child = Nodes[index];
			TBTreeNode<T, P>* sibling = Nodes[index + 1];
			child->Keys[P - 1] = Keys[index];
			for (int i = 0; i < sibling->N; ++i) {
				child->Keys[i + P] = sibling->Keys[i];
			}
			if (!child->Leaf) {
				for (int i = 0; i <= sibling->N; ++i) {
					child->Nodes[i + P] = sibling->Nodes[i];
				}
			}
			for (int i = index + 1; i < N; ++i) {
				Keys[i - 1] = Keys[i];
			}
			for (int i = index + 2; i <= N; ++i) {
				Nodes[i - 1] = Nodes[i];
			}
			child->N+= sibling->N+ 1;
			--N;
			if (sibling)
				delete sibling;
			sibling = nullptr;
		}
		void BTreeDeleteNode(T k) {//удаление ключа
			int index = BTreeFindKey(k);
			if (index <N&& Keys[index] == k) {
				if (Leaf) {
					BTreeDeleteFromLeaf(index);//?
				}
				else {
					BTreeDeleteFromNonLeaf(index);
				}
			}
			else {
				if (Leaf) {
					std::cout << "not found" << std::endl;
					return;
				}
				bool flag = ((index == N) ? true : false);
				if (Nodes[index]->N< P) {
					BTreeFill(index);//?
				}
				if (flag && index > N) {
					Nodes[index - 1]->BTreeDeleteNode(k);
				}
				else {
					Nodes[index]->BTreeDeleteNode(k);
				}
			}
		}
		void BTreePrint(std::ostream& out, int c) {//распечатать дерево
			for (int i = 0; i < c; ++i)
				std::cout << '\t';
			for (int i = 0; i < N; ++i) {
				out << Keys[i] << ' ';
			}
			out << std::endl;
			for (int i = 0; i <= N; ++i) {
				if (Nodes[i]) {
					Nodes[i]->BTreePrint(out, c + 1);
				}
			}
		}
		void BTreeDestroy() {//уничтожить дерево
			for (int i = 0; i <= N; ++i) {
				if (Nodes[i]) {
					Nodes[i]->BTreeDestroy();
					delete Nodes[i];
					Nodes[i] = nullptr;
				}
			}
		}
		void BTreeLoad(std::fstream& in) {//загрузить дерево из файла
			in.read((char*)&(this->N), sizeof(int));
			in.read((char*)&(this->Leaf), sizeof(bool));
			for (int i = 0; i < N; ++i) {
				in.read((char*)&(this->Keys[i]), sizeof(T));
			}
			if (!Leaf) {
				for (int i = 0; i <= N; ++i) {
					Nodes[i] = new TBTreeNode<T, P>();
					Nodes[i]->BTreeLoad(in);
				}
			}
		}
		void BTreeSave(std::fstream& out) {//сохранить дерево в файле
			out.write((char*)&(this->N), sizeof(int));
			out.write((char*)&(this->Leaf), sizeof(bool));
			for (int i = 0; i < N; ++i) {
				out.write((char*)&(this->Keys[i]), sizeof(T));
			}
			if (!Leaf) {
				for (int i = 0; i <= N; ++i) {
					Nodes[i]->BTreeSave(out);
				}
			}
		}
	};
public:
	TBTreeNode<T, P>* root;//корень дерева
	TBTree() {
		root = new TBTreeNode<T, P>();
	}
	~TBTree() {
		if (root) {
			root->BTreeDestroy();
			delete root;
			root = nullptr;
		}
	}
	T* BTreeSearch(T k) {
		return root->BTreeSearch(k);
	}
	void BTReeInsert(T k) {
		if (!root)
			root = new TBTreeNode<T, P>();
		if (root->N== (2 * P - 1)) {
			TBTreeNode<T, P>* s = new TBTreeNode<T, P>();
			s->Leaf = false;
			s->Nodes[0] = root;
			s->BTreeSplitChild(root, 0);
			root = s;
			root->BTreeInsertNonfull(k);
		}
		else {
			root->BTreeInsertNonfull(k);
		}
	}
	void BTreeDeleteNode(T k) {
		if (!root) {
			std::cout << "empty\n";
			return;
		}
		root->BTreeDeleteNode(k);
		if (root->N== 0) {
			TBTreeNode<T, P>* tmp = root;
			if (root->Leaf) {
				root = nullptr;
			}
			else {
				root = root->Nodes[0];
			}
			if (tmp)
				delete tmp;
		}
	}
	void BTreePrint(std::ostream& out) {
		if (root)
			root->BTreePrint(out, 0);
	}
	void BTreeSave(std::fstream& out) {
		if (root) {
			root->BTreeSave(out);
		}
	}
	void BTreeLoad(std::fstream& in) {
		if (in.peek() == EOF)
			return;
		if (root) {
			root->BTreeDestroy();
			delete root;
		}
		root = new TBTreeNode<T, P>();
		root->BTreeLoad(in);
	}
};
struct TKeyValue {
	char Key[LINE_SIZE] = "";
	unsigned long long value = 0;
	friend bool operator>(const TKeyValue& left, const TKeyValue& right);
	friend bool operator<(const TKeyValue& left, const TKeyValue& right);
	friend bool operator==(const TKeyValue& left, const TKeyValue& right);
	friend bool operator>=(const TKeyValue& left, const TKeyValue& right);
	friend bool operator<=(const TKeyValue& left, const TKeyValue& right);
	void ToLower() {
		for (int i = 0; i < LINE_SIZE; ++i) {
			if (Key[i] >= 'A' && Key[i] <= 'Z')
				Key[i] = (char)tolower((int)Key[i]);
		}
	}
};

bool operator>(const TKeyValue& left, const TKeyValue& right) {
	return (strcmp(left.Key, right.Key) > 0 ? true : false);
}
bool operator<(const TKeyValue& left, const TKeyValue& right) {
	return (strcmp(left.Key, right.Key) < 0 ? true : false);
}
bool operator==(const TKeyValue& left, const TKeyValue& right) {
	return (strcmp(left.Key, right.Key) == 0 ? true : false);
}
bool operator>=(const TKeyValue& left, const TKeyValue& right) {
	return (strcmp(left.Key, right.Key) >= 0 ? true : false);
}
bool operator<=(const TKeyValue& left, const TKeyValue& right) {
	return (strcmp(left.Key, right.Key) <= 0 ? true : false);
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	TBTree<TKeyValue, 50> btree;
	TKeyValue buffer;
	while (std::cin >> buffer.Key) {
		if (buffer.Key[0] == '+') {
			std::cin >> buffer.Key >> buffer.value;
			buffer.ToLower();
			if (btree.BTreeSearch(buffer)) {
				std::cout << "Exist\n";
				continue;
			}
			btree.BTReeInsert(buffer);
			std::cout << "OK\n";
		}
		else if (buffer.Key[0] == '-') {
			std::cin >> buffer.Key;
			buffer.ToLower();
			if (!btree.BTreeSearch(buffer)) {
				std::cout << "NoSuchWord\n";
				continue;
			}
			btree.BTreeDeleteNode(buffer);
			std::cout << "OK\n";
		}
		else if (buffer.Key[0] == '!') {
			std::cin >> buffer.Key;
			if (strcmp(buffer.Key, "Load") == 0) {
				std::cin >> buffer.Key;
				std::fstream file(buffer.Key, std::ios::binary|std::ios::in);
				if (!file.is_open()) {
					//std::cout << "ERROR: File is not opened\n";
					continue;
				}
				btree.BTreeLoad(file);
				file.close();
				std::cout << "OK\n";
			}
			else if (strcmp(buffer.Key, "Save") == 0) {
				std::cin >> buffer.Key;
				std::fstream file(buffer.Key, std::ios::binary | std::ios::out);
				file.sync_with_stdio(false);
				btree.BTreeSave(file);
				file.close();
				std::cout << "OK\n";
			}
		}
		else {
			buffer.ToLower();
			TKeyValue* result = btree.BTreeSearch(buffer);
			if (result) {
				std::cout << "OK: " << result->value << '\n';
			}
			else {
				std::cout << "NoSuchWord\n";
			}
		}
	}
	return 0;
}
