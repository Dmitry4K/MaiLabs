#pragma once
#include<iostream>
#include"vertex.h"
template <class T> class Rhombus {
private:
	Vertex<T> Vertexs[4];
public:
	using vertex_type = Vertex<T>;
	Rhombus();
	Rhombus(std::istream& in);
	Vertex<T> center() const ;
	double square() const;
	void read(std::istream& in);
	void print(std::ostream& os) const;
};

template<class T> Rhombus<T>::Rhombus() {}

template<class T> Rhombus<T>::Rhombus(std::istream& in) {
	for (int i = 0; i < 4; i++)
		in >> Vertexs[i];
}
template<class T> double Rhombus<T>::square() const {
	double Area = 0;
	for (int i = 0; i < 4; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1) % 4].y) - (Vertexs[(i + 1) % 4].x)*(Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);
}

template<class T> void Rhombus<T>::print(std::ostream& os) const {
	std::cout << "Rhombus: ";
	for (int i = 0; i < 4; i++)
		std::cout << Vertexs[i] << ' ';
	std::cout << '\n';
}

template<class T> Vertex<T> Rhombus<T>::center() const {
	Vertex<T> res = Vertex<T>();
	for (int i = 0; i < 4; i++)
		res += Vertexs[i];
	return res / 4;
}

template <class T> void Rhombus<T>::read(std::istream& in) {
	Rhombus<T> res = Rhombus<T>(in);
	*this = res;
}