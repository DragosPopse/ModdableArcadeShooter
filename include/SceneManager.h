#pragma once

#include <memory>
#include <list>
#include "Context.h"
#include "Scene.h"
#include <queue>


class SceneManager
{
	enum class ChangeType
	{
		Push,
		Pop,
		Clear
	};
	struct PendingChange
	{
		ChangeType type;
		std::shared_ptr<Scene> scene;

		PendingChange(ChangeType t, std::shared_ptr<Scene> s = nullptr) :
			type(t),
			scene(s)
		{
		}
	};

	std::list<std::shared_ptr<Scene>> _stack;
	Context* _context;
	std::queue<PendingChange> _pendingChanges;

public:
	SceneManager(Context* context);

	void PushScene(std::shared_ptr<Scene> scene);
	void PopScene();
	void Clear();

	Context* GetContext() const;

	void HandleEvent(const sf::Event& ev);
	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();

private:
	void ApplyChanges();
};