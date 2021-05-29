#include"trapeze.h"
#include<iostream>
#include<fstream>
void trapeze::read(std::istream& is) {
	for (int i = 0; i < 4; i++) {
		is >> vertices[i].x >> vertices[i].y;
	}
}
void trapeze::print(std::ostream& os) const {
	for (int i = 0; i < 4; i++) {
		os << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
}
void trapeze::print(std::string& filename) const {
	std::ofstream file;
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "File is not open" << std::endl;
		return;
	}
	file << "trapeze" << std::endl;
	for (int i = 0; i < 4; i++) {
		file << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
	file.close();
}