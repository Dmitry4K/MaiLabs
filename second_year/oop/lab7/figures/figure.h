#pragma once 
#include"../sdl.h"
#include<iostream>


struct color {
    color(): r(255), g(0), b(0) {}
    int32_t r, g, b;
    color(int r_, int g_, int b_) :r(r_), g(g_), b(b_) {}
    void set_color(int r_, int g_, int b_) { r = r_, g = g_, b = b_; }
};
struct figure {
//    figure() = 0;
    virtual void render(const sdl::renderer& renderer) const = 0;
    virtual void save(std::ostream& os) const = 0;
    virtual ~figure() = default;


    color color_{};
    virtual void set_color(int r, int g, int b) {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }
    virtual void set_color(color c) {
        color_.r = c.r;
        color_.g = c.g;
        color_.b = c.b;
    }
};


struct vertex {
    int32_t x, y;
};

