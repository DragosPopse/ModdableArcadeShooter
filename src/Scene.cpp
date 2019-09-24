#include "Scene.h"
#include "SceneManager.h"
#include "Context.h"
#include "GameObject.h"


Scene::Scene(Context& context) :
	_context(context)
{

}


Scene::~Scene()
{
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
	_context.window.draw(*_root);
}