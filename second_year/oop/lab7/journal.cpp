
#include"loader.h"
#include"figures/figures.h"
#include<stack>
#include<vector>
#include<string>
//struct journal {
   // struct j_element {
   
    void journal::push(int exp, std::unique_ptr<figure> value) {
        this->stack.push(j_element(exp, std::move(value)));
    }
    void journal::undo() {
        //stack.top();
        if (stack.size() > 0) {
            if (stack.top().exp == 1) {
                figures.pop_back();
            }
            else if (stack.top().exp == -1) {
                figures.emplace_back(std::move(stack.top().fig));
            }
            else if (stack.top().exp == 0) {
                figures = std::move(figs);
            }
            else if (stack.top().exp == 2) {
                figures.clear();
            }
            stack.pop();
        }
    }