#ifndef RECTANGLE_H
#define RECTANGLE_H

class Coordinate {
private:
	int x;
	int y;
public:
	Coordinate();
	Coordinate(int x, int y);
	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	void set(Coordinate f);
};

class Rectangle {
private:
	Coordinate first;
	Coordinate second;
public:
	Rectangle();
	Rectangle(Coordinate f, Coordinate s);
	Rectangle(int fX, int fY, int sX, int sY);
	Coordinate getFirstCoordinate();
	Coordinate getSecondCoordinate();
	int square();
	int perimetr();
	Rectangle intersection(Rectangle rec);
	Rectangle composition(Rectangle rec);
	void shift(int onX, int onY);
	void changeRectangle(Coordinate f, Coordinate s);
	bool compareSquare(Rectangle rec);
	bool comparePerimetr(Rectangle rec);
};

#endif