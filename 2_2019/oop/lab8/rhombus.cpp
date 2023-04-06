#include"rhombus.h"
#include<iostream>
#include<fstream>
void rhombus::read(std::istream& is) {
	for (int i = 0; i < 4; i++) {
		is >> vertices[i].x >> vertices[i].y;
	}
}
void rhombus::print(std::ostream& os) const {
	for (int i = 0; i < 4; i++) {
		os << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
}
void rhombus::print(std::string& filename) const {
	std::ofstream file;
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "File is not open" << std::endl;
		return;
	}
	file << "rhombus" << std::endl;
	for (int i = 0; i < 4; i++) {
		file << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
	file.close();
}