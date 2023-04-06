#pragma once

struct command {
	//command() = default;
	virtual void undo() = 0;
	virtual ~command() = default;
};