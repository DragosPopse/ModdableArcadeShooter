#include "Scene.h"
#include "GameObject.h"


Scene::Scene(Context context) :
	_context(context)
{
}


Scene::~Scene()
{
}


void Scene::SetRoot(std::unique_ptr<GameObject> root)
{
	_root.reset();
	_root = std::move(root);
}


GameObject* Scene::GetRoot()
{
	return _root.get();
}


void Scene::Update(float dt)
{
	_root->Update(dt);
}


void Scene::FixedUpdate(float dt)
{
	_root->FixedUpdate(dt);
}


void Scene::Render()
{
	_root->Draw(_context.window, sf::RenderStates::Default);
}