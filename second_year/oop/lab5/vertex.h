#pragma once
#include<iostream>
#include<cmath>
template<class T>
class Vertex {
public:
	T x, y;
	//Vertex<T>& Vertex<T>::operator=(const Vertex<T>& a);
};

template<class T>
std::istream& operator>>(std::istream& is, Vertex<T>& point) {
	is >> point.x >> point.y;
	return is;
}

template<class T>
std::ostream& operator<<(std::ostream& os, Vertex<T> point) {
	os << '[' << point.x << ",  " << point.y << ']';
	return os;
}

template<class T>
Vertex<T> operator+(const Vertex<T>& a, const Vertex<T>& b) {
	Vertex<T> res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	return res;
}
/*
template<class T>
Vertex<T>& Vertex<T>::operator=(const Vertex<T>& a) {
	x = a.x;
	y = a.y;
	return *this;
}*/

template<class T>
Vertex<T> operator+=(Vertex<T>& a, const Vertex<T>& b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

template<class T>
double distance(const Vertex<T>& a, const Vertex<T>& b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}