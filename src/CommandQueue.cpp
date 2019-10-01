#include "CommandQueue.h"


bool CommandQueue::IsEmpty() const
{
	return _queue.empty();
}


void CommandQueue::Push(const Command& command)
{
	_queue.push(command);
}


Command CommandQueue::Pop()
{
	Command command = _queue.front();
	_queue.pop();
	return command;
}