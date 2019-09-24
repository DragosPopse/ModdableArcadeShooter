#include "SceneManager.h"


SceneManager::SceneManager(Context context) :
	_context(context)
{

}


void SceneManager::PushScene(Scene* scene)
{
	std::unique_ptr<Scene> ptr(scene);

	_stack.push_back(std::move(ptr));
}


Context SceneManager::GetContext() const
{
	return _context;
}


void SceneManager::Update(float dt)
{
	_stack.back()->Update(dt);
}


void SceneManager::FixedUpdate(float dt)
{
	_stack.back()->FixedUpdate(dt);
}


void SceneManager::Render()
{
	_stack.back()->Render();
}