#include "SceneManager.h"


SceneManager::SceneManager(Context* context) :
	_context(context)
{

}


void SceneManager::PushScene(Scene* scene)
{
	std::unique_ptr<Scene> ptr(scene);

	_stack.push_back(std::move(ptr));
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
	for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
	{
		if (!(*it)->HandleEvent(ev))
		{
			return;
		}
	}
}