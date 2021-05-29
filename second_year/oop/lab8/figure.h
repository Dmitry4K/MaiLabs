#pragma once 
#include<iostream>
#include<fstream>
struct figure {
//    figure() = 0;
    virtual void read(std::istream& is) = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void print(std::string& filename) const = 0;
    virtual ~figure() = default;
};

struct vertex {
    int x, y;
};