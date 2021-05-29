#include <stdio.h>
#include <iostream>
//������ �������
template<class T> class TVector {
private:
	T* storage_;
	size_t size_;
	size_t capacity_;
public:
	TVector() : storage_(nullptr), size_(0), capacity_(0) {};
	TVector(size_t size, const T default_value = T()) {
		storage_ = new T[size];
		size_ = size;
		capacity_ = size;
		for (int i = 0; i < (int)size; i++)
			storage_[i] = default_value;
	}
	~TVector() {
		delete[] storage_;
		storage_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}
	T* Begin() const {
		return storage_;
	}
	T* End() const {
		return storage_ ? storage_ + size_ : nullptr;
	}
	size_t Size() const {
		return size_;
	}
	bool Empty() const {
		return Size() == 0;
	}
	size_t Capacity() const {
		return capacity_;
	}

	T& At(int index) const {
		if (index < 0 || index >= (int)size_)
			throw std::out_of_range("Out of range!");
		return storage_[index];
	}
	T& operator[](int index) const
	{
		return At(index);
	}
	void PushBack(T&& value) {
		if (size_ == capacity_) {
			T* temp_storage_ = storage_;
			size_ == 0 ? capacity_ = 1 : capacity_ *= 2;
			storage_ = new T[capacity_];
			for (size_t i = 0; i < size_; i++)
				storage_[i] = temp_storage_[i];
			delete[] temp_storage_;
		}
		storage_[size_++] = value;
		return;
	}
	TVector<T>& operator= (const TVector<T>& other) {
		if (Size() != other.Size()) {
			delete[] storage_;
			storage_ = new T[other.capacity_];
			capacity_ = other.capacity_;
			size_ = other.size_;
		}
		for (int i = 0; i < (int)size_; i++)
			storage_[i] = other.storage_[i];
		return *this;
	}
};


//Element - ���������, � ������ �������� ����, � ������ �������� � ������� Strings, ������������, �����
//�� ������ ������ �� ����� ������ � ������
struct Element {
public:
	char key[9];
	int index;
	Element() {}
	Element(char k[9], int ind) {
		for (int i = 0; i < 9; i++) {
			key[i] = k[i];
		}
		index = ind;
	}
};


//������� ����� String
struct String {
private:
	char str_[2049];
public:
	String() {}
	String(std::istream& in) {
		in >> str_;
	}
	friend std::ostream& operator<< (std::ostream &out, const String &point) {
		out << point.str_;
		return out;
	}
};

//���������� ������ �[256] ������������ � RadixSort
int c[256];

void RadixSort(TVector<Element>& arr) {
	TVector<Element> b = TVector<Element>(arr.Size());
	for (int j = 7; j >= 0; j--) {
		if (j == 1 || j == 5)
			continue;
		for (int i = '0'; i < 'Z' + 1; i++) {
			c[i] = 0;
		}
		for (int i = 0; i < (int)arr.Size(); i++) {
			c[(int)arr[i].key[j]]++;
		}
		for (int i = '0' + 1; i < 'Z' + 1; i++) {
			c[i] += c[i - 1];
		}
		for (int i = arr.Size() - 1; i >= 0; i--) {
			b[--c[(int)arr[i].key[j]]] = arr[i];
		}
		arr = b;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);//��������� ������������� �����
	std::cin.tie(NULL);//?
	TVector<Element> Elements;//������ � ������� �������� ����� � �������
	TVector<String> Strings;//������ � ������� �������� ������
	char key[9];//������ � ������� ������������ ����
	int i = 0;//�������
	while (std::cin>>key[0]) {//�������� �� ������ ������ � ����� �����
		std::cin.getline(key + 1, 8, '\t');//��������� ������� �����
		Elements.PushBack(Element(key, i));//���������� � ������ ����� � ���������������� ��� �������
		Strings.PushBack(String(std::cin));//�������� ������ � ���������� �� � ������
		i++;
	}

	//TVector<Element> b = TVector<Element>(Elements.Size());
	RadixSort(Elements);//���������� ���������
	for (unsigned int i = 0; i < (unsigned int)Elements.Size(); i++) {//�����   
		std::cout << Elements[i].key<<'\t' << Strings[Elements[i].index] << '\n';
	}
	return 0;
}
