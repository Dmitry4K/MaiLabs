#pragma once
#include<memory>
#include<array>
#include"figure.h"
struct  trapeze : figure {
private:
    std::array<vertex, 4> vertices;//хранилище вершин треугльника
public:
    void read(std::istream& is) override;
    void print(std::ostream& os) const override;
    void print(std::string& filename) const override;
};