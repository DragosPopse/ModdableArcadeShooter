#pragma once

/*
	Command design pattern. 
	Can be used to pass an action of every GameObject that satisfies (object.GetCategory() & command.category != 0)
	Learnt about it in the book mentioned at credits
*/

#include <functional>
#include <cassert>

#include <sol/sol.hpp>

class GameObject;

struct Command
{
	std::function<void(GameObject&, float)> action;
	unsigned int category = 0u;
};


struct LuaCommand
{
	sol::function action;
	unsigned int category = 0u;
};


template <class Derived, class Function>
std::function<void(GameObject&, float)> DeriveAction(Function func)
{
	return [=](GameObject& obj, float dt)
	{
		assert(dynamic_cast<Derived*>(&obj) != nullptr);
		func(static_cast<Derived&>(obj), dt);
	};
}