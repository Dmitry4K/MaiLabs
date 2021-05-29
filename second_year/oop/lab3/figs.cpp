#include<iostream>
#include"figs.h"
#include<cmath>
#include<cassert>
//Trapeze
Trapeze::Trapeze() {};
Trapeze::Trapeze(std::istream& in) {
	in >> Vertexs[0].x >> Vertexs[0].y >> Vertexs[1].x >> Vertexs[1].y >> Vertexs[2].x >> Vertexs[2].y >> Vertexs[3].x >> Vertexs[3].y;
	assert(vector_product(Vertexs[0] - Vertexs[3], Vertexs[1] - Vertexs[2]) == 0);
}

Vertex Trapeze::center() const {

	Vertex res;
	for (int i = 0; i<4; i++)
		res += Vertexs[i];
	return res / 4;
}

double Trapeze::square() const {
	double Area = 0;
	for (int i = 0; i < 4; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1) % 4].y) - (Vertexs[(i + 1) % 4].x)*(Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);

	/*double a, b, c;
	if (vector_product(Vertexs[0] - Vertexs[1], Vertexs[2] - Vertexs[3]) == 0) {
		a = distance(Vertexs[0], Vertexs[1]);
		b = distance(Vertexs[2], Vertexs[3]);
		c = distance(Vertexs[0], Vertexs[2]) < distance(Vertexs[0], Vertexs[3]) ? distance(Vertexs[0], Vertexs[2]) : distance(Vertexs[0], Vertexs[3]);
	}
	else if (vector_product(Vertexs[0] - Vertexs[3], Vertexs[1] - Vertexs[2]) == 0) {
		a = distance(Vertexs[0], Vertexs[3]);
		b = distance(Vertexs[1], Vertexs[2]);
		c = distance(Vertexs[0], Vertexs[1]) < distance(Vertexs[0], Vertexs[2]) ? distance(Vertexs[0], Vertexs[1]) : distance(Vertexs[0], Vertexs[2]);
	}
	else {
		a = distance(Vertexs[1], Vertexs[3]);
		b = distance(Vertexs[0], Vertexs[2]);
		c = distance(Vertexs[1], Vertexs[0]) < distance(Vertexs[1], Vertexs[2]) ? distance(Vertexs[1], Vertexs[0]) : distance(Vertexs[1], Vertexs[2]);
	}
	return ((a + b) / 2)*sqrt(pow(c, 2) - pow(((b - a) / 2), 2));*/
}

void Trapeze::printCords() const {
		std::cout << "Trapeze: ";
		for (int i = 0; i < 4; i++)
			std::cout << Vertexs[i] << ' ';
		std::cout << '\b';
}


//Rhombus
Rhombus::Rhombus() {};
Rhombus::Rhombus(std::istream& in) {
	in >> Vertexs[0].x >> Vertexs[0].y >> Vertexs[1].x >> Vertexs[1].y >> Vertexs[2].x >> Vertexs[2].y >> Vertexs[3].x >> Vertexs[3].y;
	assert((distance(Vertexs[0], Vertexs[3]) == distance(Vertexs[0], Vertexs[1])) && (distance(Vertexs[0], Vertexs[3]) == distance(Vertexs[1], Vertexs[2])) && (distance(Vertexs[0], Vertexs[3]) == distance(Vertexs[2], Vertexs[3])));
}

Vertex Rhombus::center() const {
	Vertex res = Vertex();
	for (int i = 0; i<4; i++)
		res += Vertexs[i];
	return res / 4;
}

double Rhombus::square() const {
	double Area = 0;
	for (int i = 0; i < 4; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1) % 4].y) - (Vertexs[(i + 1) % 4].x)*(Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);
}

void Rhombus::printCords() const  {
	std::cout << "Rhombus: ";
	for (int i = 0; i < 4; i++)
		std::cout << Vertexs[i] << ' ';
	std::cout << '\b';
}


//Pentagon
Pentagon::Pentagon() {};
Pentagon::Pentagon(std::istream& in) {
	in >> Vertexs[0].x >> Vertexs[0].y >> Vertexs[1].x >> Vertexs[1].y >> Vertexs[2].x >> Vertexs[2].y >> Vertexs[3].x >> Vertexs[3].y >> Vertexs[4].x >> Vertexs[4].y;
}

Vertex Pentagon::center() const {
	Vertex res = Vertex();
	for (int i = 0; i < 5; i++)
		res += Vertexs[i];
	return res / 5;
}
double Pentagon::square() const  {
	double Area = 0;
	for (int i = 0; i < 5; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1)%5].y) - (Vertexs[(i + 1)%5].x)*(Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);
}

void Pentagon::printCords() const {
	std::cout << "Pentagon: ";
	for (int i = 0; i < 5; i++)
		std::cout << Vertexs[i] << ' ';
	std::cout << '\b';
}