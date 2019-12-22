#pragma once

#include "Command.h"

#include <queue>

class CommandQueue
{
	std::queue<Command> _queue;

public:
	bool IsEmpty() const;
	Command Pop();
	void Push(const Command& command);
};