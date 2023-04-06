#include<vector>
#include<stack>
#include<memory>
#include"canvas.h"
#include"figures/figure.h"
#include"undo_commands/remove_command.h"
#include"undo_commands/add_command.h"

void canvas::add_figure(std::unique_ptr<figure> fig) {
	figures.emplace_back(std::move(fig));
	command_stack.push(std::make_unique<add_command>(add_command(this)));//?
}
void canvas::remove_figure(int id) {
	command_stack.push(std::make_unique<remove_command>(remove_command(this, std::move(figures[id]), id)));
	//figures.pop_back();
}
void canvas::undo() {
	if (command_stack.size()) {
		command_stack.top()->undo();
		command_stack.pop();
	}
}