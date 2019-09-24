#pragma once

#include <memory>
#include <list>
#include "Context.h"

class Scene;

class SceneManager
{
	std::list<std::unique_ptr<Scene>> _stack;
	//Context _context;

public:
	SceneManager();
};