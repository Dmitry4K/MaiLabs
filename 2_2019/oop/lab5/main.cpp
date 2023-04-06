#include<iostream>
#include<algorithm>
#include<clocale>
#include"trapeze.h"
#include"containers/list.h"

void Menu1() {
	std::cout << "1. Добавить фигуру в список\n";
	std::cout << "2. Удалить фигуру\n";
	std::cout << "3. Вывести фигуру\n";
	std::cout << "4. Вывести все фигуры\n";
	std::cout << "5. Вывести кол-во фигур чья площадь больше чем ...\n";
}

void PushMenu() {
	std::cout << "1. Добавить фигуру в начало списка\n";
	std::cout << "2. Добавить фигуру в конец списка\n";
	std::cout << "3. Добавить фигуру по индексу\n";
}

void DeleteMenu() {
	std::cout << "1. Удалить фигуру в начале списка\n";
	std::cout << "2. Удалить фигуру в конце списка\n";
	std::cout << "3. Удалить фигуру по индексу\n";
}

void PrintMenu() {
	std::cout << "1. Вывести первую фигуру в списке\n";
	std::cout << "2. Вывести последнюю фигуру в списке\n";
	std::cout << "3. Вывести фигуру по индексу\n";
}

int main() {
	setlocale(LC_ALL, "rus");
	containers::list<Trapeze<int>> MyList;

	Trapeze<int> TempTrapeze;

	while (true) {
		Menu1();
		int n, m, ind;
		double s;
		std::cin >> n;
		switch (n) {
		case 1:
			TempTrapeze.read(std::cin);
			PushMenu();
			std::cin >> m;
			switch (m) {
			case 1:
				MyList.push_front(TempTrapeze);
				break;
			case 2:
				MyList.push_back(TempTrapeze);
				break;
			case 3:
				std::cin >> ind;
				MyList.insert_by_number(ind, TempTrapeze);
			default:
				break;
			}
			break;
		case 2:
			DeleteMenu();
			std::cin >> m;
			switch (m) {
			case 1:
				MyList.pop_front();
				break;
			case 2:
				MyList.pop_back();
				break;
			case 3:
				std::cin >> ind;
				MyList.delete_by_number(ind);
				break;
			default:
				break;
			}
			break;
		case 3:
			PrintMenu();
			std::cin >> m;
			switch (m) {
			case 1:
				MyList.front().print(std::cout);
				std::cout << std::endl;
				break;
			case 2:
				MyList.back().print(std::cout);
				std::cout << std::endl;
				break;
			case 3:
				std::cin >> ind;
				MyList[ind].print(std::cout);
				std::cout << std::endl;
				break;
			default:
				break;
			}
			break;
		case 4:
			std::cout << MyList.length() << std::endl;
			std::for_each(MyList.begin(), MyList.end(), [](Trapeze<int>& X) { X.print(std::cout); std::cout << std::endl; });
			/*for (auto Element : MyList) {
				Element.print(std::cout);
				std::cout << std::endl;
			}*/
			break;
		case 5:
			std::cin >> s;
			std::cout << std::count_if(MyList.begin(), MyList.end(), [=](Trapeze<int>& X) {return X.square() > s; }) << std::endl;
			break;
		default:
			return 0;
		}
	}
	//*/
	system("pause");
	return 0;
}
