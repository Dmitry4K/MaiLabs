#include"pentagon.h"
#include<iostream>
#include<fstream>
void pentagon::read(std::istream& is) {
	for (int i = 0; i < 5; i++) {
		is >> vertices[i].x >> vertices[i].y;
	}
}
void pentagon::print(std::ostream& os) const {
	for (int i = 0; i < 5; i++) {
		os << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
}
void pentagon::print(std::string& filename) const {
	std::ofstream file;
	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "File is not open" << std::endl;
		return;
	}
	file << "pentagon" << std::endl;
	for (int i = 0; i < 5; i++) {
		file << vertices[i].x << ' ' << vertices[i].y << std::endl;
	}
	file.close();
}