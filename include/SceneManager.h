#pragma once

#include <memory>
#include <list>
#include "Context.h"
#include "Scene.h"


class SceneManager
{
	std::list<std::unique_ptr<Scene>> _stack;
	Context _context;

public:
	SceneManager(Context context);

	void PushScene(Scene* scene);

	Context GetContext() const;

	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();
};