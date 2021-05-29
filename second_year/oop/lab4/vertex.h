#pragma once
#include <cmath>
#include<iostream>
template <class T> class Vertex {
public:
	T x, y;
	Vertex() : x(0), y(0) {};
	Vertex(T _x, T _y) :x(_x), y(_y) {};
	Vertex& operator+=(const Vertex& b) {
		x += b.x;
		y += b.y;
		return *this;
	}
	Vertex& operator-=(const Vertex& b) {
		x -= b.x;
		y -= b.y;
		return *this;
	}
	//friend std::ostream& operator<< (std::ostream& os, const Vertex<T>& p);
	//friend std::istream& operator>> (std::istream& is, Vertex<T>& p);
};

template<class T>
std::istream& operator>> (std::istream& is, Vertex<T>& p) {
	is >> p.x >> p.y;
	return is;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const Vertex<T>& p) {
	os << p.x << ' ' << p.y;
	return os;
}

//vertex.cpp
template<class T> Vertex<T> operator+(const Vertex<T>&a, const Vertex<T>& b) {
	return Vertex<T>(a.x + b.x, a.y + b.y);
}

template<class T> Vertex<T> operator-(const Vertex<T>& a, const Vertex<T>& b) {
	return Vertex<T>(a.x - b.x, a.y - b.y);
}

template<class T> Vertex<T> operator/(const Vertex<T>& a, const int b) {
	return Vertex<T>(a.x / b, a.y / b);
}

template <class T> T distance(const Vertex<T>& a, const Vertex<T>& b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

template <class T> T vector_product(const Vertex<T>& a, const Vertex<T>& b) {
	return a.x*b.y - b.x*a.y;
}
