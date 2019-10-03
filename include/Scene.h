#pragma once

#include <memory>
#include "Context.h"
#include "ResourceHolder.h"

class SceneManager;
class GameObject;


class Scene
{
	friend class SceneManager;

	SceneManager* _manager;

protected:
	Context* _context;
	TextureHolder _textures;
	

public:
	Scene(Context* context);

	virtual ~Scene();

	virtual bool HandleEvent(const sf::Event& ev) { return false; }
	virtual bool Update(float dt) { return false; }
	virtual bool FixedUpdate(float dt) { return false; }
	virtual bool Render() { return false; }

	void RequestPush(Scene* scene);
	void RequestPop();
	void RequestClear();

	Context* GetContext() const { return _context; }
	TextureHolder& GetTextures() { return _textures; }
};