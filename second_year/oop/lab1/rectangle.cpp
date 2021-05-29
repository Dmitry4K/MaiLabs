#include "rectangle.h"

int max(int a, int b){
	return a>b ? a : b;
}

int min(int a, int b){
	return a>b ? b : a;
}

int abs(int a){
	return a>0 ? a : -a;
}


Coordinate::Coordinate() {
	this->x = 0;
	this->y = 0;
}

Coordinate::Coordinate(int x, int y) {
	this->x = x;
	this->y = y;
}

int Coordinate::getX() {
	return x;
}
int Coordinate::getY() {
	return y;
}
void Coordinate::setX(int x) {
	this->x = x;
}
void Coordinate::setY(int y) {
	this->y = y;
}
void Coordinate::set(Coordinate f) {
	this->x = f.x;
	this->y = f.y;
}

Rectangle::Rectangle() {
	this->first = Coordinate();
	this->second = Coordinate();
}
Rectangle::Rectangle(Coordinate f, Coordinate s) {
	this->first = Coordinate();
	this->second = Coordinate();
	first.set(f);
	second.set(s);
}
Rectangle::Rectangle(int fX, int fY, int sX, int sY) {
	this->first = Coordinate(fX, fY);
	this->second = Coordinate(sX, sY);
}
/*
int Rectangle::max(int a, int b) {
	return a > b ? a : b;
}

int Rectangle::min(int a, int b) {
	return a > b ? b : a;
}
*/

Coordinate Rectangle::getFirstCoordinate() {
	return this->first;
}
Coordinate Rectangle::getSecondCoordinate() {
	return this->second;
}

int Rectangle::square() {
	return abs((first.getX() - second.getX())*(first.getY() - second.getY()));
}
int Rectangle::perimetr() {
	return 2 * (abs(first.getX() - second.getX()) + abs(first.getY() - second.getY()));
}

Rectangle Rectangle::composition(Rectangle rec) {
	int left = min(first.getX(), rec.first.getX());
	int right = max(second.getX(), rec.second.getX());
	int top = max(second.getY(), rec.second.getY());
	int bottom = min(first.getX(), rec.first.getY());

	Coordinate f;
	f.setX(left);
	f.setY(bottom);
	Coordinate s;
	s.setX(right);
	s.setY(top);
	Rectangle result{ f,s };
	return result;

}
Rectangle Rectangle::intersection(Rectangle rec) {
	Rectangle result{};
	int left = max(this->first.getX(), rec.first.getX());
	int top = min(this->second.getY(), rec.second.getY());
	int right = min(this->second.getX(), rec.second.getX());
	int bottom = max(this->first.getY(), rec.first.getY());

	int width = right - left;
	int height = top - bottom;

	if (!((width < 0) || (height < 0))) {
		Coordinate f;
		f.setX(left);
		f.setY(bottom);
		Coordinate s;
		s.setX(right);
		s.setY(top);
		result.changeRectangle(f, s);
	}

	return result;
}

void Rectangle::shift(int onX, int onY) {
	first.setX(first.getX() + onX);
	second.setY(first.getY() + onY);
}
void Rectangle::changeRectangle(Coordinate f, Coordinate s) {
	first.set(f);
	second.set(s);
}
bool Rectangle::compareSquare(Rectangle rec) {
	return this->square() == rec.square();
}
bool Rectangle::comparePerimetr(Rectangle rec) {
	return this->perimetr() == rec.perimetr();
}