#include <iostream>
#include <string>
#include <fstream>
#include "rectangle.h"
#include <locale>

int main(int argc, char *argv[]) {
	int a, b, c, d;
	std::cout << "Enter Coordinate of First Rectangle: ";
	std::cin >> a >> b >> c >> d;
	Rectangle rec1 = Rectangle(a, b, c, d);
	std::cout << "Enter Coordinate of Second Rectangle: ";
	std::cin >> a >> b >> c >> d;
	Rectangle rec2 = Rectangle(a, b, c, d);
	std::cout << "First Rectangle: " << rec1 << std::endl;
	std::cout << "Second Rectangle: " << rec2 << std::endl;
	Rectangle res = Rectangle();
	res = rec1 | rec2;
	std::cout << "Composition of Rectangles: " << res << std::endl;
	res = rec1 & rec2;
	std::cout << "Intersection of Rectangles: " << res << std::endl;
	std::cout << "Increment of First Rectagngle: " << ++rec1 << std::endl;
	std::cout << "Comparison: " << (rec1 == rec2) << std::endl;
	std::cout << "Square of First Rectangle: " << (double)rec1 << std::endl;
	Rectangle rec3 = "0 1 2 3"_rtg;
	int rec3S = "0 1 2 3"_rtgSquare;
	std::cout << "Third Rectangle: " << rec3 << " Square: "<< rec3S<<std::endl;
	Rectangle rec4 = "0 1 27 0"_rtg;
	int rec4S = "0 1 27 0"_rtgSquare;
	std::cout << "Fourth Rectangle: " << rec4 << " Square: " << rec4S << std::endl;

	Rectangle rec5 = "-110 -2"_rtg;
	int rec5S = "-110 -2"_rtgSquare;
	std::cout << "Fifth Rectangle: " << rec5 << " Square: " << rec5S << std::endl;
	system("pause");
	return 0;
}