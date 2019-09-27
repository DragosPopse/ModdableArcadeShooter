#pragma once

#include <memory>
#include "Context.h"

class SceneManager;
class GameObject;


class Scene
{
protected:
	Context* _context;
	

public:
	Scene(Context* context);

	virtual ~Scene();

	virtual bool Update(float dt) { return false; }
	virtual bool FixedUpdate(float dt) { return false; }
	virtual bool Render() { return false; }
};