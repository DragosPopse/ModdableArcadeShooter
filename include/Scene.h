#pragma once

#include <memory>

class SceneManager;
class GameObject;
class Context;


class Scene
{
	std::unique_ptr<GameObject> _root;
	Context& _context;
	

public:
	Scene(Context& context);

	virtual ~Scene();

	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();
};