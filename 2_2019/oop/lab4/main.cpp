#include <iostream>
#include <tuple>

#include "Vertex.h"
#include "trapeze.h"
#include "rhombus.h"
#include "pentagon.h"
#include "templates.h"

template<class T>
void process() {
	T object;
	read(std::cin, object);
	print(std::cout, object);
	std::cout << square(object) << std::endl;
	std::cout << center(object) << std::endl;
}

int main() {
	std::cout << "How you want to declare your figure: " << std::endl;
	std::cout << "1. Tuple" << std::endl;
	std::cout << "2. Class" << std::endl;
	int menu, angles, figure;
	std::cin >> menu;
	std::cout << "How many angles (4, 5): " << std::endl;
	std::cin >> angles;
	switch (menu) {
	case 1 :
		switch (angles) {
		case 4: 
			process<std::tuple<Vertex<int>, Vertex<int>, Vertex<int>, Vertex<int>>>();
			break;
		case 5: 
			process<std::tuple<Vertex<int>, Vertex<int>, Vertex<int>, Vertex<int>, Vertex<int>>>();
			break;
		}
		break;
	case 2: 
		switch (angles) {
		case 4:
			std::cout << "What figure: " << std::endl;
			std::cout << "1. Trapeze" << std::endl;
			std::cout << "2. Rhombus" << std::endl;
			std::cin >> figure;
			switch (figure) {
			case 1:
				process<Trapeze<int>>();
				break;
			case 2:
				process<Rhombus<int>>();
				break;
			}
			break;
		case 5:
			process<Pentagon<int>>();
			break;
		}
		break;
	}
	system("pause");
	return 0;
}