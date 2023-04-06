#include <iostream>
#include <string>
#include <fstream>
#include "rectangle.h"

int main(int argc, char *argv[]){
	std::ifstream inFile(argv[1]);
	int a,b,c,d;
	inFile >> a >> b >> c >> d;
	Rectangle rec1 = Rectangle(a,b,c,d);
	inFile >> a >> b >> c >> d;
	Rectangle rec2 = Rectangle(a,b,c,d);
	inFile.close();
	std::string argument1 = argv[1];
	std::string name_file = "result_" + argument1;
	std::ofstream outFile;
	outFile.open(name_file);
	outFile << "First Rectangle:"<< std::endl;
	outFile << "	First Coordinate: X: "<< rec1.getFirstCoordinate().getX()<<" Y: "<< rec1.getFirstCoordinate().getY()<<std::endl;
	outFile << "	Second Coordinate: X: "<< rec1.getSecondCoordinate().getX()<<" Y: "<< rec1.getSecondCoordinate().getY()<<std::endl;
	outFile << "	Square: "<< rec1.square()<< " Perimetr : "<<rec1.perimetr()<<std::endl;

	outFile << "Second Rectangle:"<< std::endl;
	outFile << "	First Coordinate: X: "<< rec2.getFirstCoordinate().getX()<<" Y: "<< rec2.getFirstCoordinate().getY()<<std::endl;
	outFile << "	Second Coordinate: X: "<< rec1.getSecondCoordinate().getX()<<" Y: "<< rec1.getSecondCoordinate().getY()<<std::endl;
	outFile << "	Square: "<< rec2.square()<< " Perimetr : "<<rec2.perimetr()<<std::endl;

	outFile << "Compare on Square: " << rec1.compareSquare(rec2)<< " Compare on Perimetr: " << rec1.comparePerimetr(rec2) << std::endl;
	outFile << "Intersection : "<<std::endl;
	Rectangle rec3 = rec1.intersection(rec2);
	outFile << "	First Coordinate: X: "<< rec3.getFirstCoordinate().getX()<<" Y: "<< rec3.getFirstCoordinate().getY()<<std::endl;
	outFile << "	Second Coordinate: X: "<< rec3.getSecondCoordinate().getX()<<" Y: "<< rec3.getSecondCoordinate().getY()<<std::endl;
	outFile << "	Square: "<< rec3.square()<< " Perimetr : "<<rec3.perimetr()<<std::endl;

	outFile << "Composition : "<<std::endl;
	rec3 = rec1.composition(rec2);
	outFile << "	First Coordinate: X: "<< rec3.getFirstCoordinate().getX()<<" Y: "<< rec3.getFirstCoordinate().getY()<<std::endl;
	outFile << "	Second Coordinate: X: "<< rec3.getSecondCoordinate().getX()<<" Y: "<< rec3.getSecondCoordinate().getY()<<std::endl;
	outFile << "	Square: "<< rec3.square()<< " Perimetr : "<<rec3.perimetr()<<std::endl;

	return 0;
}