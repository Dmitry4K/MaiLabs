#pragma once
#include<iostream>
class Vertex {
public:
	double x, y;
	Vertex();
	Vertex(double _x, double _y);
	Vertex& operator+=(const Vertex& b);
	Vertex& operator-=(const Vertex& b);
	friend std::ostream& operator<< (std::ostream &out, const Vertex &point);
};
Vertex operator+ (const Vertex &a, const Vertex& b);
Vertex operator- (const Vertex &a, const Vertex& b);
Vertex operator/ (const Vertex &a, const double& b);
double distance(const Vertex &a, const Vertex& b);
double vector_product(const Vertex& a, const Vertex& b);