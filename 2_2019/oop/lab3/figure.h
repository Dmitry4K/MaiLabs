#pragma once
#include<iostream>
#include<vector>
#include"vertex.h"
class Figure {
public:
	virtual Vertex center() const = 0;
	virtual double square() const = 0;
	virtual void printCords() const = 0;
	//virtual ~Figure();
};