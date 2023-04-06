#pragma once 
#include<memory>
#include"command.h"
#include"../figures/figure.h"
#include"../canvas.h"
struct remove_command :command {
public:
	
	void undo() override;
	//remove_command() {}
	remove_command(canvas* canvas_, std::unique_ptr<figure> fig_, int id_);
private:
	canvas* canvas_p;
	int id;
	std::unique_ptr<figure> fig;
};