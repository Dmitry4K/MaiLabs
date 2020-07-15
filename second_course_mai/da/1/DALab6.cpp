#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include<iomanip>


const long long DEFAULT_BASE_FOR_LONG_NUMBER = 1000000;

class TLongNumber {
private:
	long long base = DEFAULT_BASE_FOR_LONG_NUMBER;
	std::vector<long long> container;
public:
	TLongNumber();
	TLongNumber(const std::string& str);
	TLongNumber(long long a);
	void Clear();
	void Print(std::ostream& out) const;
	friend int Cmp(const TLongNumber& a, const TLongNumber& b);
	friend bool operator>(const TLongNumber& a, const TLongNumber& b);
	friend bool operator<(const TLongNumber& a, const TLongNumber& b);
	friend bool operator>=(const TLongNumber& a, const TLongNumber& b);
	friend bool operator<=(const TLongNumber& a, const TLongNumber& b);
	friend bool operator==(const TLongNumber& a, const TLongNumber& b);
	friend bool operator!=(const TLongNumber& a, const TLongNumber& b);

	friend TLongNumber Sum(const TLongNumber& a, const TLongNumber& b);//a > 0 , b > 0
	friend TLongNumber Sub(const TLongNumber& a, const TLongNumber& b);// a - b > 0
	friend TLongNumber Mult(const TLongNumber& a, const TLongNumber& b);
	friend TLongNumber Div(const TLongNumber& a, const TLongNumber& b);
	friend TLongNumber SmallDiv(const TLongNumber& a, const int& b);
	friend TLongNumber SmallMult(const TLongNumber& a, const int& b);
	friend std::ostream& operator<< (std::ostream& out, const TLongNumber& a);
	friend TLongNumber Exp(const TLongNumber& a, const TLongNumber&);
	friend TLongNumber SmallExp(const TLongNumber& a, const TLongNumber& b);
};
TLongNumber SmallMult(const TLongNumber& a, const int& b);
TLongNumber Exp(const TLongNumber& a, const TLongNumber&);
TLongNumber SmallDiv(const TLongNumber& a, const int& b);
int Cmp(const TLongNumber& a, const TLongNumber& b);
TLongNumber Sum(const TLongNumber& a, const TLongNumber& b);
TLongNumber Sub(const TLongNumber& a, const TLongNumber& b);
TLongNumber Mult(const TLongNumber& a, const TLongNumber& b);
TLongNumber Div(const TLongNumber& a, const TLongNumber& b);
std::ostream& operator<< (std::ostream& out, const TLongNumber& a);

std::ostream& operator<< (std::ostream& out, const TLongNumber& a) {
	a.Print(out);
	return out;
}

TLongNumber::TLongNumber() {}
TLongNumber::TLongNumber(const std::string& s) {
	int k = (int)std::log10(base);
	for (int i = (int)s.length(); i > 0; i -= k)
		if (i < k)
			container.push_back(atoi(s.substr(0, i).c_str()));
		else
			container.push_back(atoi(s.substr(i - k, k).c_str()));

	while (container.size() > 1 && container.back() == 0)
		container.pop_back();
}
void TLongNumber::Print(std::ostream& out) const {
	out << (container.empty() ? 0 : container.back());
	int k = (int)log10(base);
	for (int i = (int)container.size() - 2; i >= 0; --i)
		out << std::setw(k) << std::setfill('0') << container[i];
}
TLongNumber::TLongNumber(long long a) {
	if (a == 0) {
		container.push_back(0);
	}
	while (a > 0) {
		container.push_back(a % base);
		a /= base;
	}
}
void TLongNumber::Clear() {
	while (container.size() > 1 && container[container.size() - 1] == 0)
		container.pop_back();
}

int Cmp(const TLongNumber& a, const TLongNumber& b) {
	if (a.container.size() != b.container.size()) {
		return a.container.size() > b.container.size() ? 1 : -1;
	}
	for (int i = a.container.size() - 1; i >= 0; --i) {
		if (a.container[i] != b.container[i]) {
			return a.container[i] > b.container[i] ? 1 : -1;
		}
	}
	return 0;
}

bool operator>(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) == 1 ? true : false;
}
bool operator<(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) == -1 ? true : false;
}
bool operator>=(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) >= 0 ? true : false;
}
bool operator<=(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) <= 0 ? true : false;
}
bool operator==(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) == 0 ? true : false;
}
bool operator!=(const TLongNumber& a, const TLongNumber& b) {
	return Cmp(a, b) != 0 ? true : false;
}

TLongNumber Sum(const TLongNumber& a, const TLongNumber& b) {
	long long transfer = 0;
	long long size = std::max(a.container.size(), b.container.size());
	long long s = 0;
	TLongNumber C;
	for (int i = 0; i < size; ++i) {
		if (i >= (int)a.container.size()) {
			s = b.container[i] + transfer;
			C.container.push_back(s % a.base);
			transfer = s / a.base;
		}
		else if (i >= (int)b.container.size()) {
			s = a.container[i] + transfer;
			C.container.push_back(s % a.base);
			transfer = s / a.base;
		}
		else {
			s = a.container[i] + b.container[i] + transfer;
			C.container.push_back(s % a.base);
			transfer = s / a.base;
		}
	}
	if (transfer != 0) {
		C.container.push_back(transfer);
	}
	return C;
}


TLongNumber Sub(const TLongNumber& a, const TLongNumber& b) {
	if (a < b)
		return Sub(b, a);
	TLongNumber c;
	long long transfer = 0;
	long long s;
	for (int i = 0; i < (int)a.container.size(); ++i) {
		if (i >= (int)b.container.size()) {
			s = a.container[i] - transfer;
		}
		else {
			s = a.container[i] - b.container[i] - transfer;
		}
		transfer = 0;
		if (s < 0) {
			s += a.base;
			transfer = 1;
		}
		c.container.push_back(s);
	}
	c.Clear();
	return c;
}
TLongNumber Mult(const TLongNumber& a, const TLongNumber& b) {
	TLongNumber c;
	c.container.resize(a.container.size() + b.container.size());
	for (size_t i = 0; i < a.container.size(); ++i)
		for (int j = 0, carry = 0; j < (int)b.container.size() || carry; ++j) {
			long long cur = c.container[i + j] + a.container[i] * 1ll * (j < (int)b.container.size() ? b.container[j] : 0) + carry;
			c.container[i + j] = int(cur % a.base);
			carry = int(cur / a.base);
		}
	while (c.container.size() > 1 && c.container.back() == 0)
		c.container.pop_back();
	return c;
}
TLongNumber Div(const TLongNumber& a, const TLongNumber& b) {
	TLongNumber res, cv = TLongNumber(0);
	res.container.resize(a.container.size());
	for (int i = (int)a.container.size() - 1; i >= 0; --i) {
		cv.container.insert(cv.container.begin(), a.container[i]);
		if (!cv.container.back())
			cv.container.pop_back();
		int x = 0, l = 0, r = a.base;
		while (l <= r) {
			int m = (l + r) / 2;
			TLongNumber cur = SmallMult(b, m);
			if (cur <= cv) {
				x = m;
				l = m + 1;
			}
			else {
				r = m - 1;
			}
		}
		res.container[i] = x;
		cv = Sub(cv, SmallMult(b, x));
	}

	while (res.container.size() > 1 && res.container.back() == 0)
		res.container.pop_back();
	return res;
}
TLongNumber SmallDiv(const TLongNumber& a, const int& b) {
	TLongNumber c;
	c.container.resize(a.container.size());
	int carry = 0;
	for (int i = (int)a.container.size() - 1; i >= 0; --i) {
		long long cur = a.container[i] + carry * 1ll * a.base;
		c.container[i] = int(cur / b);
		carry = int(cur % b);
	}
	while (c.container.size() > 1 && c.container.back() == 0)
		c.container.pop_back();
	return c;
}
TLongNumber SmallMult(const TLongNumber& a, const int& b) {
	int carry = 0;
	TLongNumber c;
	c.container = std::vector<long long>(a.container);
	for (size_t i = 0; i < c.container.size() || carry; ++i) {
		if (i == c.container.size())
			c.container.push_back(0);
		long long cur = carry + c.container[i] * 1ll * b;
		c.container[i] = int(cur % c.base);
		carry = int(cur / c.base);
	}
	while (c.container.size() > 1 && c.container.back() == 0)
		c.container.pop_back();
	return c;
}

TLongNumber Exp(const TLongNumber& a, const TLongNumber& b) {
	if (a == 1) {
		return 1;
	}
	if (b == 0) {
		return 1;
	}
	if (b == 1) {
		return a;
	}
	if (b == 2) {
		return Mult(a, a);
	}
	TLongNumber r = SmallDiv(b, 2);
	TLongNumber l = Sub(b, r);
	return Mult(Exp(a, l), Exp(a, r));
}

TLongNumber SmallExp(const TLongNumber& a, const TLongNumber& b) {
	TLongNumber res(1);
	TLongNumber b1(b);
	TLongNumber a1(a);
	while (b1 > 0) {
		if (b1.container.back() % 2)
			res = Mult(res, a1);
		a1 = Mult(a1, a1);
		b1 = SmallDiv(b1, 2);
	}
	return res;
}


int main() {
	std::cin.tie(NULL);
	std::ios::sync_with_stdio(false);
	std::string str1;
	std::string str2;
	char Operator;
	while (std::cin >> str1 >> str2 >> Operator) {
		TLongNumber a(str1);
		TLongNumber b(str2);
		switch (Operator) {
		case '+':
			std::cout << Sum(a, b) << std::endl; std::cin.get();
			break;
		case '-':
			if (a >= b) {
				std::cout << Sub(a, b) << std::endl; std::cin.get();
			}
			else {
				std::cout << "Error\n"; std::cin.get();
			}
			break;
		case '*':
			std::cout << Mult(a, b) << std::endl; std::cin.get();
			break;
		case '^':
			if (a == 0 && b == 0) {
				std::cout << "Error\n"; std::cin.get();
			}
			else {
				std::cout << SmallExp(a, b) << std::endl; std::cin.get();
			}
			break;
		case '/':
			if (b != 0) {
				std::cout << Div(a, b) << std::endl; std::cin.get();
			}
			else {
				std::cout << "Error\n"; std::cin.get();
			}
			break;
		case '>':
			std::cout << (a > b ? "true\n" : "false\n"); std::cin.get();
			break;
		case '<':
			std::cout << (a < b ? "true\n" : "false\n"); std::cin.get();
			break;
		case '=':
			std::cout << (a == b ? "true\n" : "false\n"); std::cin.get();
			break;
		}
	}
	return 0;
}
