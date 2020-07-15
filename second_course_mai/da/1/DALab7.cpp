#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>

struct TCell{
	long long value ;
	long long j;
	TCell() : value(-1), j(-1){}
	TCell(long long _value, long long _j) : value(_value), j(_j) {}
	friend bool operator< (const TCell& a, const TCell& b); 
	friend bool operator> (const TCell& a, const TCell& b);
	friend bool operator<= (const TCell& a, const TCell& b);
	friend bool operator>= (const TCell& a, const TCell& b);
	friend bool operator== (const TCell& a, const TCell& b);
	friend TCell operator+ (const TCell& a, const long long b);
};

TCell operator+ (const TCell& a, const long long b) {
	TCell res;
	res.value = a.value + b;
	res.j = a.j;
	return res;
}
bool operator< (const TCell& a, const TCell& b) {
	return a.value < b.value;
}
bool operator> (const TCell& a, const TCell& b) {
	return a.value > b.value;
}
bool operator<= (const TCell& a, const TCell& b) {
	return a.value <= b.value;
}
bool operator>=(const TCell& a, const TCell& b) {
	return a.value >= b.value;
}
bool operator== (const TCell& a, const TCell& b) {
	return a.value == b.value;
}
TCell MyMin(const TCell a, const TCell b) {
	return (a < b) ? a : b;
}
int main() {
	std::ios::sync_with_stdio(false);

	//������ n � m
	long long n, m;
	std::cin >> n >> m;

	//������� ����
	std::vector<std::vector<long long>> field;
	std::vector<std::vector<TCell>> myField;
	field.resize(n);
	myField.resize(n);
	for (int i = 0; i < n; ++i) {
		field[i].resize(m);
		myField[i].resize(m);
	}
	//������ ����
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cin >> field[i][j];
		}
	}
	for (int i = 0; i < m; ++i) {
		myField[myField.size() - 1][i] = TCell(field[field.size()-1][i], -1);
	}
	for (int i = n - 2; i >= 0; --i) {
		for (int j = 0; j < m; ++j) {
			if (j == 0) {
				myField[i][j] = MyMin(TCell(myField[i + 1][j].value, j), TCell(myField[i + 1][j + 1].value,j+1)) + field[i][j];
			}
			else if (j == m - 1) {
				myField[i][j] = MyMin(TCell(myField[i + 1][j].value, j), TCell(myField[i + 1][j - 1].value, j-1)) + field[i][j];
			}
			else {
				myField[i][j] = MyMin(TCell(myField[i + 1][j - 1].value,j-1), MyMin(TCell(myField[i + 1][j].value, j), TCell(myField[i + 1][j + 1].value, j+1))) + field[i][j];
			}
		}
	} 
	TCell min(myField[0][0]);
	long long minJ = 0;
	for (int i = 1; i < m; ++i) {
		if (min > myField[0][i]) {
			min.value = myField[0][i].value;
			min.j = myField[0][i].j;
			minJ = i;
		}
	}
	std::cout << min.value << std::endl;
	std::cout <<'('<< 1<< ',' << minJ + 1<< ") ";
	for (int i = 1; i < n-1; ++i) {
		std::cout << '(' << i + 1 << ',' << min.j + 1<< ") ";
		min.j = myField[i][min.j].j;
	}
	std::cout << '(' << n << ',' << min.j + 1 << ")" << std::endl;
	return 0;
}
