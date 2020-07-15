#pragma once
#include<iostream>
#include<set>
#include<queue>

struct IdAndSocket {
	int Id;
	int SocketNumber;

	friend bool operator== (const IdAndSocket& c1, const IdAndSocket& c2);

	friend bool operator<= (const IdAndSocket& c1, const IdAndSocket& c2);

	friend bool operator>= (const IdAndSocket& c1, const IdAndSocket& c2);

	friend bool operator< (const IdAndSocket& c1, const IdAndSocket& c2);

	friend bool operator> (const IdAndSocket& c1, const IdAndSocket& c2);

	friend bool operator!= (const IdAndSocket& c1, const IdAndSocket& c2);
	friend std::ostream& operator<< (std::ostream& out, const IdAndSocket& point);
};


std::ostream& operator<< (std::ostream& out, const IdAndSocket& point) {
	out << point.Id << ' ' << point.SocketNumber;
	return out;
 }
bool operator== (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id == c2.Id;
}

bool operator<= (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id <= c2.Id;
}

bool operator>= (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id >= c2.Id;
}
bool operator< (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id < c2.Id;
}
bool operator> (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id > c2.Id;
}
bool operator!= (const IdAndSocket& c1, const IdAndSocket& c2) {
	return c1.Id != c2.Id;
}

struct MyBSTree {
	using T = IdAndSocket;
	struct MyBSTreeNode;
	MyBSTreeNode* _Root = nullptr;
	std::queue<T> UsedSockets = std::queue<T>();
	int NewSocketNumber = 2;

public:
	struct MyBSTreeNode {
		T _Key;
		bool _Empty = true;
		MyBSTreeNode* _Left = nullptr;
		MyBSTreeNode* _Right = nullptr;
		MyBSTreeNode(T Key) : _Key(Key), _Empty(false) {}
		MyBSTreeNode() {}
		void Add(T Key) {
			if (Key < _Key) {//добавляем в левого потомка 
				if (_Left == nullptr) {
					_Left = new MyBSTreeNode(Key);
				}
				else {
					(*_Left).Add(Key);
				}
			}
			else {
				if (_Right == nullptr) {
					_Right = new MyBSTreeNode(Key);
				}
				else {
					(*_Right).Add(Key);
				}
			}
		}
		bool Empty() {
			return _Empty;
		}
		bool Exist(const T& Key) {
			if (Key == _Key)
				return true;
			if (Key < _Key) {
				if (_Left == nullptr) {
					return false;
				}
				else return _Left->Exist(Key);
			}
			else {
				if (_Right == nullptr) {
					return false;
				}
				else {
					return _Right->Exist(Key);
				}
			}
		}
		void Size(int& S) {
			if (!_Empty) {
				++S;
			}
			if (_Left != nullptr) {
				_Left->Size(S);
			}
			if (_Right != nullptr) {
				_Right->Size(S);
			}
		}
		void Destroy(std::queue<T>* UsedSockets) {
			if (_Left != nullptr)
				_Left->Destroy(UsedSockets);
			if (_Right != nullptr)
				_Right->Destroy(UsedSockets);
			UsedSockets->push(_Key);
			delete _Left;
			delete _Right;
			_Empty = true;
		}
		void Delete(const T& Key, std::queue<T>* UsedSockets) {
			if (_Key == Key) {
				Destroy(UsedSockets);
			}
			else {
				if (Key < _Key) {
					if (_Left != nullptr) {
						_Left->Delete(Key, UsedSockets);
						_Left = nullptr;
					}
				}
				else {
					if (_Right != nullptr) {
						_Right->Delete(Key, UsedSockets);
						_Right = nullptr;
					}
				}
			}
		}
		void Print(std::ostream& in, int tabs) {
			for (int i = 0; i < tabs; ++i) {
				in << '\t';
			}
			in << _Key << std::endl;
			if (_Left != nullptr) {
				_Left->Print(in, tabs + 1);
			}
			if (_Right != nullptr) {
				_Right->Print(in, tabs + 1);
			}
		}
		void GetNodes(std::vector<int>& res) {
			if(!_Empty)
				res.push_back(_Key.Id);
			if (_Left != nullptr) {
				_Left->GetNodes(res);
			}
			if (_Right != nullptr) {
				_Right->GetNodes(res);
			}
		}
		T Find(T Key) {
			if (_Key == Key) {
				return _Key;
			}
			else {
				T res{ -1,-1 };
				if (_Left) {
					res = _Left->Find(Key);
				}
				if (_Right && res != T{ -1,-1 }) {
					res = _Right->Find(Key);
				}
				return res;
			}
		}
		T FindParent(T Key) {
			if (_Left) {
				if (_Left->_Key == Key) {
					return _Key;
				}
			}
			else if (_Right) {
				if (_Right->_Key == Key) {
					return _Key;
				}
			}
			else {
				T res = T{ -1, -1 };
				if (_Left) {
					res = _Left->FindParent(Key);
				}
				if (_Right && res == T{-1, -1}) {
					res = _Right->FindParent(Key);
				}
				return res;
			}
		}
	};
	MyBSTree() {
		_Root = new MyBSTreeNode();
	}
	MyBSTree(T Key) {
		_Root = new MyBSTreeNode(Key);
	}
	int Size() {
		int res = 0;
		_Root->Size(res);
		return res;
	}
	std::vector<int> GetNodes() {
		std::vector<int> Res;
		if (_Root != nullptr) {
			if (!(_Root->_Empty)) {
				_Root->GetNodes(Res);
			}
		}
		return Res;
	}
	void Add(T Key) {
		if (_Root->Empty()) {
			_Root->_Key = Key;
			_Root->_Empty = false;
		}
		else {
			if (Key < _Root->_Key) {
				if (_Root->_Left == nullptr) {
					_Root->_Left = new MyBSTreeNode(Key);
				}
				else {
					_Root->_Left->Add(Key);
				}
			}
			else {
				if (_Root->_Right == nullptr) {
					_Root->_Right = new MyBSTreeNode(Key);
				}
				else {
					_Root->_Right->Add(Key);
				}
			}
		}
	}
	bool Exist(const T& Key) {
		if (_Root->Empty()) {
			return false;
		}
		return _Root->Exist(Key);
	}
	void Delete(const T Key) {
		if (_Root->_Key == Key) {
			_Root->Destroy(&UsedSockets);
		}
		else {
			_Root->Delete(Key, &UsedSockets);
		}
	}
	void Destroy() {
		if (!_Root->Empty())
			_Root->Destroy(&UsedSockets);
	}
	void Print(std::ostream& in) {
		if (!_Root->Empty()) {
			_Root->Print(in, 0);
		}
	}
	T Find(T Key) {
		if (_Root) {
			if (_Root->_Key == Key) {
				return _Root->_Key;
			}
			return _Root->Find(Key);
		}
		else return T{ -1, -1 };
	}
	T FindParent(T Key) {
		if (_Root) {
			return _Root->FindParent(Key);
		}
		else return T{ -1,-1 };
	}
	T GenNewSocketNumber() {
		if (UsedSockets.empty()) {
			NewSocketNumber++;
			T res{ -1,  NewSocketNumber };
			return res;
		}
		else {
			T res = UsedSockets.front();
			UsedSockets.pop();
			return res;
		}
	}
};