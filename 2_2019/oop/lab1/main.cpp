#include <iostream>
#include "rectangle.h"

using namespace std;

int main() {
	Rectangle rectangle = Rectangle(1,2,3,4);

	Rectangle rectangle2 = Rectangle(3,4,1,2);


	cout << rectangle.square() << endl;
	cout << rectangle.perimetr() << endl;
	cout << rectangle2.compareSquare(rectangle)<<endl;
	system("pause");
	return 0;
}