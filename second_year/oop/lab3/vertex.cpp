#pragma once
#include"vertex.h"
#include<cmath>

Vertex::Vertex(): x(0),y(0) {}
Vertex::Vertex(double _x, double _y): x(_x), y(_y) {}
Vertex& Vertex::operator+=(const Vertex& b) {
	x += b.x;
	y += b.y;
	return *this;
}
Vertex& Vertex::operator-=(const Vertex& b) {
	x -= b.x;
	y -= b.y;
	return *this;
}
Vertex operator+(const Vertex &a, const Vertex& b) {
	return Vertex(a.x + b.x, a.y + b.y);
}

Vertex operator-(const Vertex &a, const Vertex& b) {
	return Vertex(a.x - b.x, a.y - b.y);
}

Vertex operator/(const Vertex &a, const double& b) {
	return Vertex(a.x / b, a.y / b);
}

double distance(const Vertex &a, const Vertex& b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double vector_product(const Vertex& a, const Vertex& b) {
	return a.x*b.y - b.x*a.y;
}

std::ostream& operator<< (std::ostream &out, const Vertex &point) {
	out << "[" << point.x << ", " << point.y << ']';
	return out;
}