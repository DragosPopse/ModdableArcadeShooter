#pragma once

#include <memory>
#include <list>
#include "Context.h"
#include "Scene.h"
#include <queue>

class Engine;


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
	Engine* _engine;

public:
	SceneManager(Context* context);

	void PushScene(std::shared_ptr<Scene> scene);
	void PopScene();
	void Clear();

	void ResetLua();
	void SetEngine(Engine* engine);

	bool IsEmpty() const { return _stack.empty(); }

	Context* GetContext() const;

	void HandleEvent(const sf::Event& ev);
	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();


	const Scene* Top() const;

private:
	void ApplyChanges();
};