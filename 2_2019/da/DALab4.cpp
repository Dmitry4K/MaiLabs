#include<iostream>
#include<vector> 
#include<algorithm>
#include<cmath>

const size_t BUFFER_SIZE = 1000000; //размер буффера

std::vector<long long> prefix_function(std::vector<unsigned long int>& pattern)
{
	long long n = (long long)pattern.size();
	std::vector<long long> pi(n); // в i-м элементе (его индекс i-1) количество совпавших символов в начале и конце для подстроки длины i. 
			 // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых 
	for (long long i = 1; i < n; ++i)
	{
		// ищем, какой префикс-суффикс можно расширить
		long long j = pi[i - 1]; // длина предыдущего префикса-суффикса, возможно нулевая
		while ((j > 0) && (pattern[i] != pattern[j])) // этот нельзя расширить,
			j = pi[j - 1];   // берем длину меньшего префикса-суффикса

		if (pattern[i] == pattern[j])
			++j;  // расширяем найденный (возможно пустой) префикс-суффикс
		pi[i] = j;
	}
	return pi;
}

void KMP(std::vector<long long>& pf, std::vector<unsigned long int>& pattern, std::vector<unsigned long int>& text, int begin, std::vector<long long>& result){
	for (long long k = 0, i = begin; i < text.size(); ++i) {
		while ((k > 0) && (pattern[k] != text[i])) {
			k = pf[k - 1]; //сдвигаем по след значению по префикс функции
		}
		if (pattern[k] == text[i]) {
			k++; //продолжаем сравнение если буквы совпали
		}
		if (k == pattern.size()) {
			k = pf[k-1]; //если найдено вхождение, то сдвигаем по след значению префикс функции
			result.push_back(i - pattern.size() + 1); //в вектор пушится индекс вхождения
		}
	}
}
//работает когда размер буффера минимум больше размера патрена в 2 раза
void PrintResults(std::vector<long long>& result, std::vector<long long>& lines, long long& lines_count, long long pattern_size, long long&start_off) {
	long long prev_line = 0;
	for (int i = 0, j = 0, k = 0; i < BUFFER_SIZE; ++i) {

		if (j < lines.size() ) {
			if (i == lines[j]) {//есть по индексу в буффере есть переход на новую строку то уведичиваем кол-во считанных строк
				while (i == lines[j]) {
					++lines_count;  //запоминаем этот индекс
					++j;
					if (j >= lines.size()) {
						break;
					}
				}
				--j;
				prev_line = lines[j];
				++j;           //ждем след индекс на котором был переход на новую строку
				start_off = 0; //больше не учитываем заданный в начале сдвиг
				pattern_size = 0;//? больше не учитываем сдвиг на pattern_size элементов
			}
		}
		if (k < result.size()) {
			if (i == result[k]) {
				long long b = result[k] - prev_line + start_off - pattern_size; //индекс на котором начинается вхождение
				if (b < 0) { //если отрицательный, то возвращаемся на предыдущую строку
					std::cout << lines_count << ", " << BUFFER_SIZE + b << std::endl; //выводим строку и скорректированный индекс
				}
				else {
					std::cout << lines_count + 1 << ", " << b + 1 << std::endl; //выводим строку и индекс, + 1 так как отсчет начинается с 1
				}
				++k; 
			}
		}	
	}
	start_off += BUFFER_SIZE - prev_line - pattern_size; //запоминаем сдвиг для следующего вывода

}

template<class T> void PrintVector(std::vector<T> vector) {
	for (int i = 0; i < vector.size(); ++i) {
		std::cout << vector[i] << ' ';
	}	
	std::cout << std::endl;
}

int main() {
	std::cin.sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::vector<unsigned long int> pattern; //паттерн 
	std::vector<unsigned long int> buffer;
	buffer.reserve(BUFFER_SIZE);//буффер в котором хранится часть файла
	std::vector<unsigned long int> tmp;
	unsigned long int word; //word - переменная в которой хранится считанное число(буква)
	std::vector<long long> result, lines;
	long long m = 0;
	// получаем паттерн ----------------------------

	while (std::cin >> word) {
		pattern.push_back(word);
		while(std::cin.peek() == ' '){
			std::cin.get();
		}
		if (std::cin.peek() == '\n') {
			std::cin.get();
			break;
		}
	}

	//----------------------------------------------

	std::vector<long long> pf = prefix_function(pattern); //подсчет префикс функции для патерна
	bool FirstTime = true;
	long long lines_count = 0 , prev = 0;
	// получаем возможные пустые строки в начале ---
	while (std::cin.peek() == ' ') {
		std::cin.get();
	}
	while (std::cin.peek() == '\n') {
		std::cin.get();
		lines.push_back(buffer.size());
	}

	// считывание части файла и ее обработка -------

	while (std::cin >> word) {
		//-------------------------------------------
		buffer.push_back(word);
		++m;
		if (m > BUFFER_SIZE - pattern.size()) {
			tmp.push_back(word);
		}
		//-------------------------------------------

		if (buffer.size() == BUFFER_SIZE) {
			if (FirstTime) { // если считываем первые раз, то поиск начинаем с самого первого элемента
				KMP(pf, pattern, buffer, 0, result);
				PrintResults(result, lines, lines_count, 0,  prev);
				FirstTime = false;
			}
			else { // иначе, так как в буффер записана предыдущая, ранее уже проверенная часть, начинаем поиск со 2 элемента
				KMP(pf, pattern, buffer, 1, result);
				PrintResults(result, lines, lines_count, (int)pattern.size(), prev);
			}
			buffer = tmp; // сохраняем часть по размеру равную патерну
			m = buffer.size(); // кол-во обработанных циклом символов
			tmp.clear();
			lines.clear();
			result.clear();
		}
		while (std::cin.peek() == ' ') {
			std::cin.get();
		}
		while (std::cin.peek() == '\n') {
			std::cin.get();
			lines.push_back(buffer.size());
		}
	}
	// ---------------------------------------------


	//обработка оставщейся части -------------------
	
	if (FirstTime) {
		KMP(pf, pattern, buffer, 0, result);
		PrintResults(result, lines, lines_count, 0,  prev);
	}
	else {
		KMP(pf, pattern, buffer, 1, result);
		PrintResults(result, lines, lines_count, (int)pattern.size(), prev);
	}
	
	// ---------------------------------------------

	return 0;
}
