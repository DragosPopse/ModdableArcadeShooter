#include "SceneManager.h"

#include "Scenes/Level.h"
#include "Scenes/LevelLoader.h"

#include "Utility/RangeFor.h"


SceneManager::SceneManager(Context* context) :
	_context(context)
{

}


void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	scene->_manager = this;
	_pendingChanges.push(PendingChange(ChangeType::Push, std::move(scene)));
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
	for (auto& scene : ReverseFor(_stack))
	{
		if (!scene->Update(dt))
		{
			return;
		}
	}
}


void SceneManager::FixedUpdate(float dt)
{
	ApplyChanges();
	for (auto& scene : ReverseFor(_stack))
	{
		if (!scene->FixedUpdate(dt))
		{
			return;
		}
	}
}


void SceneManager::Render()
{
	for (auto& scene : _stack)
	{
		if (!scene->Render())
		{
			return;
		}
	}
}


void SceneManager::HandleEvent(const sf::Event& ev)
{
	ApplyChanges();
	for (auto& scene : ReverseFor(_stack))
	{
		if (!scene->HandleEvent(ev))
		{
			return;
		}
	}
}


void SceneManager::ApplyChanges()
{
	bool c = false;
	while (!_pendingChanges.empty())
	{
		c = true;
		PendingChange change = _pendingChanges.front();
		_pendingChanges.pop();
		switch (change.type)
		{
		case ChangeType::Push:
			{
				std::shared_ptr<Scene> ptr(change.scene);
				ptr->Enable();
				_stack.push_back(std::move(ptr));
			}
			break;

		case ChangeType::Pop:
			if (!_stack.empty())
			{
				_stack.back()->Disable();
				_stack.pop_back();
			}
			break;

		case ChangeType::Clear:
			if (!_stack.empty())
			{
				for (auto& scene : ReverseFor(_stack))
				{
					scene->Disable();
				}
				_stack.clear();
			}
			break;
		}
	}
}


const Scene* SceneManager::Top() const
{
	return _stack.back().get();
}