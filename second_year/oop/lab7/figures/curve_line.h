#pragma once
#include"tools.h"
#include"figure.h"
#include<vector>
#include<memory>
struct  curve_line : figure {
    curve_line(const std::vector<vertex>& vertices) : vertices_(vertices) {}

    void render(const sdl::renderer& renderer) const override {//рисует фигуру
        renderer.set_color(color_.r, color_.g, color_.b);
        for (int32_t i = 0; i < vertices_.size()-1; ++i) {
            renderer.draw_line(vertices_[i].x, vertices_[i].y,
                vertices_[(i + 1)].x, vertices_[(i + 1)].y);
        }
    }

    void save(std::ostream& os) const override {//сохраняет фигуру
        os << "curve_line" <<' '<<vertices_.size()<< std::endl;
        for (int32_t i = 0; i < vertices_.size(); ++i) {
            os << vertices_[i].x << ' ' << vertices_[i].y << '\n';
        }
        os << this->color_.r << ' '<< this->color_.g<< ' ' << this->color_.b << std::endl;
    }

private:
    std::vector<vertex> vertices_;//хранилище вершин треугльника

};

struct  curve_line_builder : builder {//строитель треугольника
    std::unique_ptr<figure> add_vertex(const vertex& v) {//добавить вершну V в треугольник, наверно еще не построенный
        vertices_.push_back(v);
        n_ += 1;
        if (n_ < 2)
            return nullptr;

        if (vertices_[(n_ - 1)].x != vertices_[(n_ - 2)].x && vertices_[(n_ - 1)].y != vertices_[(n_ - 2)].y)
            return nullptr;
       // jl.push(1, nullptr);
        return std::make_unique<curve_line>(vertices_);
    }

private:
    int32_t n_ = 0; //кол-во вершин.
    std::vector<vertex> vertices_;
};