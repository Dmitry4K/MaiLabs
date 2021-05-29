#include "rectangle.h"
#include <sstream>
int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a > b ? b : a;
}

int abs(int a) {
	return a > 0 ? a : -a;
}


Coordinate::Coordinate() {
	this->x = 0;
	this->y = 0;
}

Coordinate::Coordinate(int x, int y) {
	this->x = x;
	this->y = y;
}

int Coordinate::getX() const {
	return x;
}
int Coordinate::getY() const {
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

Coordinate& Coordinate::operator+=(const Coordinate& b) {
	x += b.x;
	y += b.y;
	return *this;
}

Coordinate& Coordinate::operator++() {
	++x;
	++y;
	return *this;
}
Coordinate& Coordinate::operator++(int) {
	Coordinate result = Coordinate(this->x++, this->y++);
	*this = result;
	return *this;
}

Coordinate& Coordinate::operator+=(const int& b) {
	x += b;
	y += b;
	return *this;
}

Coordinate operator+(const Coordinate& a, const Coordinate& b) {
	Coordinate result = Coordinate(a);
	result += b;
	return result;
}

Coordinate operator+(const Coordinate &a, int &b) {
	Coordinate result = Coordinate(a);
	result += b;
	result += b;
	return result;
}

Coordinate operator+(int& a, const Coordinate& b) {
	Coordinate result = Coordinate(b);
	result += a;
	result += a;
	return result;
}

Coordinate& Coordinate::operator-=(const int& b) {
	x -= b;
	y -= b;
	return *this;
}

Coordinate& Coordinate::operator-=(const Coordinate& b) {
	x -= b.x;
	y -= b.y;
	return *this;
}

Coordinate operator-(const Coordinate& a, int& b) {
	Coordinate result = Coordinate(a);
	result -= b;
	result -= b;
	return result;
}
Coordinate operator-(int& a, const Coordinate& b) {
	Coordinate result = Coordinate(b);
	result -= a;
	result -= a;
	return result;
}
Coordinate& Coordinate::operator--() {
	--x;
	--y;
	return *this;
}
Coordinate& Coordinate::operator--(int) {
	Coordinate result = Coordinate(this->x--,this->y--);
	*this = result;
	return *this;
}

std::ostream& operator<< (std::ostream &out, const Coordinate &point) {
	out << "X: " << point.x << " Y: " << point.y;
	return out;
}


std::istream& operator>> (std::istream &in, Coordinate &point) {
	in >> point.x >> point.y;
	return in;
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

Coordinate Rectangle::getFirstCoordinate() const {
	return this->first;
}
Coordinate Rectangle::getSecondCoordinate() const {
	return this->second;
}

int Rectangle::square() const{
	return abs((first.getX() - second.getX())*(first.getY() - second.getY()));
}
int Rectangle::perimetr() const {
	return 2 * (abs(first.getX() - second.getX()) + abs(first.getY() - second.getY()));
}

Rectangle Rectangle::composition(const Rectangle rec) const {
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
Rectangle Rectangle::intersection(const Rectangle rec) const {
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

Rectangle operator+(const Rectangle& a, int& b) {
	Rectangle result = Rectangle(a);
	result += b;
	return result;
}
Rectangle operator+(int& b,const  Rectangle& a) {
	Rectangle result = Rectangle(a);
	result += b;
	return result;
}

Rectangle& Rectangle::operator++() {
	++first;
	++second;
	return *this;
}
Rectangle& Rectangle::operator++(int) {
	Rectangle result = Rectangle(this->first++, this->second++);
	*this = result;
	return *this;
}

Rectangle operator-(const Rectangle& a, int& b) {
	Rectangle result = Rectangle(a);
	result += b;
	return result;
}
Rectangle operator-(int& b, const Rectangle& a) {
	Rectangle result = Rectangle(a);
	result += b;
	return result;
}

Rectangle& Rectangle::operator--() {
	--first;
	--second;
	return *this;
}
Rectangle& Rectangle::operator--(int) {
	Rectangle result = Rectangle(this->first--, this->second--);
	*this = result;
	return *this;
}

Rectangle operator+(const Rectangle &a, const Rectangle& b) {
	Rectangle result = Rectangle(a);
	result += b;
	return result;
}
Rectangle& Rectangle::operator+=(const Rectangle& b) {
	first += b.first;
	second += b.second;
	return *this;
}

Rectangle& Rectangle::operator+=(const int& b) {
	first += b;
	second += b;
	return *this;
}
Rectangle& Rectangle::operator-=(const Rectangle& b) {
	first -= b.first;
	second -= b.second;
	return *this;
}
Rectangle& Rectangle::operator-=(const int& b) {
	first -= b;
	second -= b;
	return *this;
}
Rectangle operator-(const Rectangle &a, const Rectangle& b) {
	Rectangle result = Rectangle(a);
	result -= b;
	return result;
}

bool operator>(const Rectangle &a,const Rectangle& b) {
	return a.square() > b.square() ? true : false;
}
bool operator== (const Rectangle &a,const  Rectangle& b) {
	return a.square() == b.square() ? true : false;
}
bool operator<(const Rectangle &a,const Rectangle& b) {
	return a.square() < b.square() ? true : false;
}
bool operator<=(const Rectangle &a,const Rectangle& b) {
	return a.square() <= b.square() ? true : false;
}
bool operator>=(const Rectangle &a,const  Rectangle& b) {
	return a.square() >= b.square() ? true : false;
}

std::ostream& operator<< (std::ostream &out, const Rectangle &point) {
	out << "First coordinate: " << point.first << " Second coordinate: " << point.second;
	return out;
}

std::istream& operator>> (std::istream &in, Rectangle &point) {
	in >> point.first >> point.second;
	return in;
}

Rectangle::operator double () {
	int result = this->square();
	return result;
}

/*Rectangle operator "" _rtg(const char* str, size_t size) {
	bool reg = false;
	int * a = new int[4];
	for (int i = 0; i < 4; i++)
		a[i] = 0;
	int count = 0, number = 0;
	for (int i = 0; i < (int)size; i++) {
		if (str[i] == ':') {
			if ((count + 1) == 4)
				break;
			if (reg) number = -number;
			a[count] = number;
			count++;
			number = 0;
			reg = false;
		}
		else {
			if (str[i] == '-')
				reg = true;
			else {
				number *= 10;
				number += str[i] - '0';
			}
		}
	}
	if (reg) number = -number;
	a[count] = number;
		return Rectangle(a[0],a[1],a[2],a[3]);
	
}*/

Rectangle operator&(const Rectangle &a,const  Rectangle& b) {
	return Rectangle(a.intersection(b));
}
Rectangle operator| (const Rectangle &a,const Rectangle& b) {
	return Rectangle(a.composition(b));
}

Rectangle operator "" _rtg(const char* str, size_t size) {
	std::istringstream is(str);
	Rectangle result = Rectangle();
	is >> result;
	return result;
}


int operator "" _rtgSquare(const char* str, size_t size) {
	std::istringstream is(str);
	Rectangle result = Rectangle();
	is >> result;
	return result.square();
}