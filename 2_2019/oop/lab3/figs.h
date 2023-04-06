#pragma once
#include<iostream>
#include"figure.h"
class Trapeze : public Figure {
private:
	Vertex Vertexs[4];
public:
	Trapeze();
	Trapeze(std::istream& in);
	Vertex center() const override;

	double square() const override;

	void printCords() const override;
};

class Rhombus : public Figure {
private:
	Vertex Vertexs[4];
public:
	Rhombus();
	Rhombus(std::istream& in);
	
	Vertex center() const override;

	double square() const override;

	void printCords() const override;
};

class Pentagon : public Figure {
private:
	Vertex Vertexs[5];
public:
	Pentagon();
	Pentagon(std::istream& in);
	
	Vertex center() const override;

	double square() const override;

	void printCords() const override;
};