#pragma once
#include<vector>
#include"figure.h"
struct handler {
	virtual void execute(std::vector<std::unique_ptr<figure>>& figures ) = 0;
	virtual ~handler() = default;
};