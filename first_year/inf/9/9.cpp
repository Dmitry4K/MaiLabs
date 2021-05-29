#include<stdio.h>
#include<locale.h>
#include<iostream>
#include<math.h>
bool result(int i, int j) {
	if ((sqrt((i+20)*(i+20) + (j+20) * (j+20)) <= 10) && (sqrt((i + 20)*(i + 20) + (j + 10) * (j + 10)) <= 10) && (i <= -10) && (j <= -10) && (i >= -20) && (j >= -20))
		return 1;
	else return 0;
}
int sign(int a) {
	if (a < 0)
		return -1;
	if (a > 0)
		return 1;
	if (a == 0)
		return 0;
}
int min(int a, int b)
{
	if (a < b)
		return a;
	else return b;
}
int max(int a, int b)
{
	if (a < b)
		return b;
	else return a;

}
int main() {
	const int i0 = -22;
	const int j0 = 29;
	const int l0 = 4;
	setlocale(LC_ALL, "rus");
	int i, j, l,ik,jk,lk;
	ik = i0; jk = j0; lk = l0;
	for (int k = 1; k < 51; k++) {
		if (result(ik, jk)) {
			printf("Шарик попал в лунку.\n");
			printf("Время: %d\t Координата i: %d\t Координата j: %d\t Параметр движения l: %d\t\n", k-1,ik, jk, lk);
			system("pause");
			return 0;
		}
		i = sign(min(ik, jk))*max((ik + k) % 20, (jk + lk) % 20);
		j = abs(max(ik, jk)) - k * min(jk, lk);
		l = (k - lk) / ((ik + jk + lk)*(ik + jk + lk) % 5 + 1);
		printf("Шаг %d\t i: %d\t j: %d\t l: %d\t\n", k, i, j, l, sign(min(i0, jk)));

		ik = i; jk = j; lk = l;
	}
	printf("Шарик не попал в лунку, сделано больше 50 шагов.\n");
	printf("Время: 50\t Координата i: %d\t Координата j: %d\t Параметр движения l: %d\t\n",i,j,l);
	system("pause");
	return 0;
	}