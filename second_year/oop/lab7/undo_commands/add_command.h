#pragma once 
#include<vector>
#include"command.h"
#include"../canvas.h"

struct add_command : command {
private:
	canvas* canvas_p;
public:
	//add_command() {}
	add_command(canvas* canvas_) {
		canvas_p = canvas_;
	}
	void undo() override {
		(*canvas_p).figures.pop_back();
	}
};