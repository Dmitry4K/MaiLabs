#pragma once 
#include"figures/figures.h"
#include<vector>
#include<memory>


struct loader {
    std::vector<std::unique_ptr<figure>> load(std::ifstream& is);
    ~loader() = default;
};