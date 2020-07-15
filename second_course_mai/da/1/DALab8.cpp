#include<iostream>
#include<vector>
#include<cmath>
#include<iomanip>
#include<algorithm>

double SquareOfTriangle(long long a, long long b, long long c) {
	double p = (double)(double(a) + double(b) + double(c)) / double(2);
	return sqrt(p * (p - double(a)) * (p - double(b)) * (p - double(c)));
}
bool IsTriangle(long long a, long long b, long long c) {
	if (((a + b > c) && (b + c > a)) && (c + a > b)) {
		return true;
	}
	else {
		return false;
	}
}
void CountingSort(std::vector<long long>& a, long long max_el) {
	std::vector<long long> c(max_el + 1);
	for (long long i = 0; i <= max_el; i++)
		c[i] = 0;
	for (long long i = 0; i < a.size(); i++)
		c[a[i]]++;
	for (long long i = 1; i <= max_el; i++)
		c[i] += c[i - 1];
	std::vector<long long> b(a.size());
	for (long long i = a.size() - 1; i >= 0; i--) {
		c[a[i]]--;
		b[c[a[i]]] = a[i];
	}
	for (long long i = 0; i < a.size(); i++)
		a[i] = b[i];
}
int main() {
	std::ios::sync_with_stdio(false);
	long long n;
	std::cin >> n;
	std::vector<long long> sides(n);
	std::cin >> sides[0];
	long long max = sides[0];
	for (long long i = 1; i < n; ++i) {
		std::cin >> sides[i];
		if (max < sides[i]) {
			max = sides[i];
		}
	}
	CountingSort(sides, max);
	bool isFound = false;
	long long first = 0, second = 0, third = 0;
	double Square = 0;
	for (long long i = sides.size() - 1; i >= 2; --i) {
		for (long long j = i - 1; j >= 1; --j) {
			for (long long k = j - 1; k >= 0; --k) {
				if (IsTriangle(sides[i], sides[j], sides[k])) {
					first = sides[k];
					second = sides[j];
					third = sides[i];
					Square = SquareOfTriangle(sides[i], sides[j], sides[k]);
					isFound = true;
					break;
				}
			}
			if (isFound) {
				break;
			}
		}
		if (isFound) {
			break;
		}
	}
	for (int i = sides.size() - 1; i >= 2; --i) {
		if (IsTriangle(sides[i], sides[i - 1], sides[i - 2])) {
			if (Square < SquareOfTriangle(sides[i], sides[i - 1], sides[i - 2])) {
				Square = SquareOfTriangle(sides[i], sides[i - 1], sides[i - 2]);
				first = sides[i - 2];
				second = sides[i - 1];
				third = sides[i];
			}
		}
	}
	if (isFound) {
		std::cout << std::fixed << std::setprecision(3) << Square << std::endl;
		std::cout << first << ' ' << second << ' ' << third << std::endl;
	}
	else {
		std::cout << 0 << std::endl;
	}
	return 0;
}
