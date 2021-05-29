#ifndef RECTANGLE_H
#define RECTANGLE_H
#include<iostream>

class Coordinate {
private:
	int x;
	int y;
public:
	Coordinate();
	Coordinate(int x, int y);
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
	void set(Coordinate f);
	Coordinate& operator+=(const Coordinate& b);
	Coordinate& operator+=(const int& b);
	Coordinate& operator-=(const int& b);
	Coordinate& operator-=(const Coordinate& b);
	Coordinate& operator++();
	Coordinate& operator--();
	Coordinate& operator--(int);
	Coordinate& operator++(int);
	friend std::ostream& operator<< (std::ostream &out, const Coordinate &point);
	friend std::istream& operator>> (std::istream &in, Coordinate &point);
};

Coordinate operator+(const Coordinate& a, const Coordinate& b);
Coordinate operator+(const Coordinate& a, int& b);
Coordinate operator+(int& a, const Coordinate& b);
Coordinate operator-(const Coordinate& a, int& b);
Coordinate operator-(int& a, const Coordinate& b);

class Rectangle {
protected:
	Coordinate first;
	Coordinate second;
public:
	Rectangle();
	Rectangle(Coordinate f, Coordinate s);
	Rectangle(int fX, int fY, int sX, int sY);
	Coordinate getFirstCoordinate() const;
	Coordinate getSecondCoordinate() const;
	int square() const;
	int perimetr() const;
	Rectangle intersection(const Rectangle rec) const;
	Rectangle composition(const Rectangle rec) const;
	void shift(int onX, int onY);
	void changeRectangle(Coordinate f, Coordinate s);
	bool compareSquare(Rectangle rec);
	bool comparePerimetr(Rectangle rec);

	Rectangle& operator++();
	Rectangle& operator++(int);
	Rectangle& operator+=(const Rectangle& b);
	Rectangle& operator-=(const Rectangle& b);
	Rectangle& operator+=(const int& b);
	Rectangle& operator-=(const int& b);
	Rectangle& operator--();
	Rectangle& operator--(int);
	friend bool operator>(const Rectangle &a, const Rectangle& b);
	friend bool operator== (const Rectangle &a, const Rectangle& b);
	friend bool operator<(const Rectangle &a, const Rectangle& b);
	friend bool operator<=(const Rectangle &a,const Rectangle& b);
	friend bool operator>=(const Rectangle &a,const Rectangle& b);

	friend std::ostream& operator<< (std::ostream &out, const Rectangle &point);
	friend std::istream& operator>> (std::istream &in, Rectangle &point);
	operator double ();
};

Rectangle operator-(const Rectangle &a, const Rectangle& b);
Rectangle operator+(const Rectangle& a, int& b);
Rectangle operator+(int& a, const Rectangle& b);
Rectangle operator-(const Rectangle& a, int& b);
Rectangle operator-(int& a, const Rectangle& b);
Rectangle operator+(const Rectangle &a, const Rectangle& b);

Rectangle operator&(const Rectangle &a, const Rectangle& b);
Rectangle operator| (const Rectangle &a, const Rectangle& b);
Rectangle operator "" _rtg(const char* str,size_t size);
int operator "" _rtgSquare(const char* str, size_t size);
#endif