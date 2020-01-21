#pragma once

/*
	Scene Manger that functions as a stack. You can push and pop scenes. Multiple scenes can be updated at the same time, in the
order of the stack, or reverse when talking about rendering.
	Changes to the stack are queued and applied at the same time, to ensure there won't be any weird dealocations in the middle of the loop.
	Scenes are stored in shared_ptr because some scenes function with the help of another.
*/

#include <memory>
#include <list>
#include <queue>

#include "Context.h"
#include "Scene.h"

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

public:
	SceneManager(Context* context);

	void PushScene(std::shared_ptr<Scene> scene);
	void PopScene();
	void Clear();


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