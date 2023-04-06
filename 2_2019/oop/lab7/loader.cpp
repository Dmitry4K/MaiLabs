
#include"loader.h"
#include"figures/figures.h"
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<memory>

    std::vector<std::unique_ptr<figure>> loader::load(std::ifstream& is) {
        std::string figure_name;
        std::vector<std::unique_ptr<figure>> figures;
        while (is >> figure_name) {
            vertex v;
            if (figure_name == std::string("trapeze")) {
                std::array<vertex, 4> vertices;
                for (int i = 0; i < 4; ++i) {
                    is >> v.x >> v.y;
                    vertices[i] = v;
                }
                struct color t_color {};
                is >> t_color.r >> t_color.g >> t_color.b;
                figures.emplace_back(std::make_unique<trapeze>(vertices));
                (*figures[figures.size() - 1]).set_color(t_color);
            }
            else if (figure_name == std::string("pentagon")) {
                std::array<vertex, 5> vertices;
                for (int i = 0; i < 5; ++i) {
                    is >> v.x >> v.y;
                    vertices[i] = v;
                }
                struct color t_color {};
                is >> t_color.r >> t_color.g >> t_color.b;
                figures.emplace_back(std::make_unique<pentagon>(vertices));
                (*figures[figures.size() - 1]).set_color(t_color);
            }
            else if (figure_name == std::string("rhombus")) {
                std::array<vertex, 4> vertices;
                for (int i = 0; i < 4; ++i) {
                    is >> v.x >> v.y;
                    vertices[i] = v;
                }
                struct color t_color {};
                is >> t_color.r >> t_color.g >> t_color.b;
                figures.emplace_back(std::make_unique<rhombus>(vertices));
                (*figures[figures.size() - 1]).set_color(t_color);
            }
            else if (figure_name == std::string("line")) {
                std::array<vertex, 2> vertices;
                for (int i = 0; i < 2; ++i) {
                    is >> v.x >> v.y;
                    vertices[i] = v;
                }
                struct color t_color {};
                is >> t_color.r >> t_color.g >> t_color.b;
                figures.emplace_back(std::make_unique<line>(vertices));
                (*figures[figures.size() - 1]).set_color(t_color);
            }

            else if (figure_name == std::string("curve_line")) {
                std::vector<vertex> vertices;
                int count_v;
                is >> count_v;
                for (int i = 0; i < count_v; ++i) {
                    is >> v.x >> v.y;
                    vertices.push_back(v);
                }
                struct color t_color {};
                is >> t_color.r >> t_color.g >> t_color.b;
                figures.emplace_back(std::make_unique<curve_line>(vertices));
                (*figures[figures.size() - 1]).set_color(t_color);
            }
        }
        return figures;
    }