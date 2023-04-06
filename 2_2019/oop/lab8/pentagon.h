#pragma once
#include<memory>
#include<array>
#include<string>
#include"figure.h"
struct  pentagon : figure {

private:
    std::array<vertex, 5> vertices;//��������� ������ �����������
public:
    void read(std::istream& is) override;
    void print(std::ostream& os) const override;
    void print(std::string& filename) const override;
};