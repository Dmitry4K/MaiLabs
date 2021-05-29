#pragma once
#include"vertex.h"
template <class T>
class Trapeze {
private:
	Vertex<T> Vertexs[4];
public:
	using vertex_type = Vertex<T>;
	Trapeze() = default;
	Trapeze(std::istream& in);
	void read(std::istream& in);
	Vertex<T> center() const;
	double square() const;
	void print(std::ostream& os) const;
	friend std::ostream& operator<< (std::ostream& out, const Trapeze<T>& point);
	friend std::ostream& operator>> (std::istream& in, const Trapeze<T>& point);
};

//template<class T> Trapeze<T>::Trapeze() {}

template<class T> Trapeze<T>::Trapeze(std::istream& in) {
	for (int i = 0; i < 4; i++)
		in >> Vertexs[i];
}
template<class T> double Trapeze<T>::square() const {
	double Area = 0;
	for (int i = 0; i < 4; i++) {
		Area += (Vertexs[i].x) * (Vertexs[(i + 1) % 4].y) - (Vertexs[(i + 1) % 4].x) * (Vertexs[i].y);
	}
	Area *= 0.5;
	return abs(Area);
}

template<class T> void Trapeze<T>::print(std::ostream& os) const {
	os << "Trapeze: ";
	for (int i = 0; i < 4; i++)

		os << Vertexs[i] << ' ';
	os << '\n';
}


template<class T> Vertex<T> Trapeze<T>::center() const {
	Vertex<T> res = Vertex<T>();
	for (int i = 0; i < 4; i++)
		res += Vertexs[i];
	return res / 4;
}

template <class T> void Trapeze<T>::read(std::istream& in) {
	Trapeze<T> res = Trapeze(in);
	*this = res;
}

template<class T>
std::ostream& operator<< (std::ostream& out, const Trapeze<T>& point) {
	out << "Trapeze: ";
	for (int i = 0; i < 4; i++)
		out << point.Vertexs[i] << ' ';
	out << '\n';
	return out;
}

template<class T>
std::istream& operator>> (std::istream& in, const Trapeze<T>& point) {
	for (int i = 0; i < 4; i++)
		in >> point.Vertexs[i];
	return in;
}