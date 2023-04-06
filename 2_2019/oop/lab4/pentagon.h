#pragma once
#include<iostream>
#include"vertex.h"
template <class T> class Pentagon{
private:
	Vertex<T> Vertexs[5];
public:
	using vertex_type = Vertex<T>;
	Pentagon();
	Pentagon(std::istream& in);
	Vertex<T> center() const;
	double square() const;
	void read(std::istream& in);
	void print(std::ostream& os) const;
};

template<class T>
Pentagon<T>::Pentagon() {}

template<class T> Pentagon<T>::Pentagon(std::istream& in) {
	for (int i = 0; i < 5; i++)
		in >> Vertexs[i];
}
template<class T> double Pentagon<T>::square() const {
	double Area = 0;
	for (int i = 0; i < 5; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1) % 5].y) - (Vertexs[(i + 1) % 5].x)*(Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);
}

template<class T> void Pentagon<T>::print(std::ostream& os) const {
	std::cout << "Pentagon: ";
	for (int i = 0; i < 5; i++)
		std::cout << Vertexs[i] << ' ';
	std::cout << '\n';
}

template<class T> Vertex<T> Pentagon<T>::center() const {
	Vertex<T> res = Vertex<T>();
	for (int i = 0; i < 5; i++)
		res += Vertexs[i];
	return res / 5;
}

template <class T> void Pentagon<T>::read(std::istream& in) {
	Pentagon<T> res = Pentagon(in);
	*this = res;
}