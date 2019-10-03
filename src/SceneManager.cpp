#include "SceneManager.h"


SceneManager::SceneManager(Context* context) :
	_context(context)
{

}


void SceneManager::PushScene(Scene* scene)
{
	scene->_manager = this;
	_pendingChanges.push(PendingChange(ChangeType::Push, scene));
}


void SceneManager::PopScene()
{
	_pendingChanges.push(PendingChange(ChangeType::Pop));
}


void SceneManager::Clear()
{
	_pendingChanges.push(PendingChange(ChangeType::Clear));
}


Context* SceneManager::GetContext() const
{
	return _context;
}


void SceneManager::Update(float dt)
{
	for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
	{
		if (!(*it)->Update(dt))
		{
			return;
		}
	}
}


void SceneManager::FixedUpdate(float dt)
{
	for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
	{
		if (!(*it)->FixedUpdate(dt))
		{
			return;
		}
	}
}


void SceneManager::Render()
{
	for (auto it = _stack.begin(); it != _stack.end(); ++it)
	{
		if (!(*it)->Render())
		{
			return;
		}
	}
}


void SceneManager::HandleEvent(const sf::Event& ev)
{
	ApplyChanges();
	for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
	{
		if (!(*it)->HandleEvent(ev))
		{
			return;
		}
	}
}


void SceneManager::ApplyChanges()
{
	while (!_pendingChanges.empty())
	{
		PendingChange change = _pendingChanges.front();
		_pendingChanges.pop();
		switch (change.type)
		{
		case ChangeType::Push:
			{
				std::unique_ptr<Scene> ptr(change.scene);
				_stack.push_back(std::move(ptr));
			}
			break;

		case ChangeType::Pop:
			if (!_stack.empty())
			{
				_stack.pop_back();
			}
			break;

		case ChangeType::Clear:
			_stack.clear();
			break;
		}
	}
}