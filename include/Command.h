#pragma once

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