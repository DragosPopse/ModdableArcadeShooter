#pragma once

#include <memory>
#include "Context.h"

class SceneManager;
class GameObject;


class Scene
{
	std::unique_ptr<GameObject> _root; //root object for all the entities in the scene
	std::unique_ptr<GameObject> _canvas; //root object for all the UI
	Context _context;
	

public:
	Scene(Context context);

	virtual ~Scene();

	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();

	void SetRoot(std::unique_ptr<GameObject> root);
	GameObject* GetRoot();
};