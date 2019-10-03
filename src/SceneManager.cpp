#include "SceneManager.h"
#include "Scenes/Level.h"
#include "Scenes/LevelLoader.h"


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
	ApplyChanges();
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
	ApplyChanges();
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
	if (!_pendingChanges.empty())
	{
		std::cout << "WHATAFAC: " << _pendingChanges.size() << "\n";
	}
	bool c = false;
	while (!_pendingChanges.empty())
	{
		c = true;
		std::cout << (int)_pendingChanges.front().type << " ";
		PendingChange change = _pendingChanges.front();
		_pendingChanges.pop();
		switch (change.type)
		{
		case ChangeType::Push:
			{
				std::unique_ptr<Scene> ptr(change.scene);
				_stack.emplace_back(std::move(ptr));
			}
			break;

		case ChangeType::Pop:
			if (!_stack.empty())
			{
				std::cout << "#" << " ";
				_stack.pop_back();
			}
			break;

		case ChangeType::Clear:
			if (!_stack.empty())
			{
				std::cout << "#" << " ";
				_stack.clear();
			}
			break;
		}
	}
	if (c)
	{
		std::cout << '\n';
	}
}