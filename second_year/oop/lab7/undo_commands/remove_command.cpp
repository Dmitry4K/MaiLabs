
#include<memory>
#include"command.h"
#include"../figures/figure.h"
#include"../canvas.h"
#include"remove_command.h"
void remove_command::undo() {
	(*canvas_p).figures[id] = std::move(fig);
}
remove_command::remove_command(canvas* canvas_, std::unique_ptr<figure> fig_, int id_) {
		canvas_p = canvas_;
		fig = std::move(fig_);
		id = id_;
}