#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"


Scene::Scene(Context* context) :
	_context(context)
{
}


Scene::~Scene()
{
}


void Scene::RequestPush(std::shared_ptr<Scene> scene)
{
	_manager->PushScene(scene);
}


void Scene::RequestPop()
{
	_manager->PopScene();
}


void Scene::RequestClear()
{
	_manager->Clear();
}