#pragma once
#include<cmath>
#include<memory>
#include<stack>
#include<string>
#include<iostream>
#include<vector>
#include<array>
#include"sdl.h"
#include"figures/figure.h"


struct builder {
    virtual std::unique_ptr<figure> add_vertex(const vertex& v) = 0;//функция добавления новой вершины в фигуру

    virtual ~builder() = default;//деструктор, ненужная фигня, но должна быть

};
