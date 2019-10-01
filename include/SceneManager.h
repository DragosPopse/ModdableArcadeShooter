#pragma once

#include <memory>
#include <list>
#include "Context.h"
#include "Scene.h"


class SceneManager
{
	enum ChangeType
	{
		Push,
		Pop,
		Clear
	};
	struct PendingChange
	{
		ChangeType type;
		Scene* scene;

		PendingChange(ChangeType t, Scene* s = nullptr) :
			type(t),
			scene(s)
		{
		}
	};

	std::list<std::unique_ptr<Scene>> _stack;
	Context* _context;

public:
	SceneManager(Context* context);

	void PushScene(Scene* scene);
	void PopScene();

	Context* GetContext() const;

	void HandleEvent(const sf::Event& ev);
	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();

private:
	void ApplyChanges();
};