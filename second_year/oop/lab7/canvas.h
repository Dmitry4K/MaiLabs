#pragma once
#include<vector>
#include<stack>
#include<memory>
#include"figures/figure.h"
#include"undo_commands/command.h"
struct canvas {
	std::vector<std::unique_ptr<figure>> figures;
	std::stack<std::unique_ptr<command>> command_stack;
	void add_figure(std::unique_ptr<figure> fig);
	void remove_figure(int id);
	void undo();
};