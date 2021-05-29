#pragma once
#include"tools.h"
#include"figure.h"
#include<memory>
struct  trapeze : figure {
     trapeze(const std::array<vertex, 4>& vertices) : vertices_(vertices) {}

    void render(const sdl::renderer& renderer) const override {//рисует фигуру
        renderer.set_color(color_.r, color_.g, color_.b);
        for (int32_t i = 0; i < 4; ++i) {
            renderer.draw_line(vertices_[i].x, vertices_[i].y,
                vertices_[(i + 1) % 4].x, vertices_[(i + 1) % 4].y);
        }
    }

    void save(std::ostream& os) const override {//сохраняет фигуру
        os << "trapeze" << std::endl;
        for (int32_t i = 0; i < 4; ++i) {
            os << vertices_[i].x << ' ' << vertices_[i].y << '\n';
        }

        os << this->color_.r << ' ' << this->color_.g << ' ' << this->color_.b << std::endl;
    }

private:
    std::array<vertex, 4> vertices_;//хранилище вершин треугльника

};



struct  trapeze_builder : builder {//строитель треугольника
private:
    int32_t n_ = 0; //кол-во вершин.
    std::array<vertex, 4> vertices_;
public:
    std::unique_ptr<figure> add_vertex(const vertex& v) {//добавить вершну V в треугольник, наверно еще не построенный
        vertices_[n_] = v;
        n_ += 1;
        if (n_ != 4) {
            return nullptr;
        }
      //  jl.push(1, nullptr);
        return std::make_unique<trapeze>(vertices_);
    }

};
