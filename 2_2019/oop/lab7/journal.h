#pragma once
#include"figures/figures.h"
#include<stack>
#include<vector>
#include<string>
struct journal {
    struct j_element {
        int exp;
        std::unique_ptr<figure> fig;
        j_element(int _exp, std::unique_ptr<figure> _fig) : exp(_exp), fig(std::move(_fig)) {}
    };
    std::stack<j_element> stack;
    std::vector<std::unique_ptr<figure>> figs;
    void push(int exp, std::unique_ptr<figure> value);
    void undo();
};
static journal jl;