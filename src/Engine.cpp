#include <iostream>

#include "Engine.h"
#include "LuaInit.h"
#include "Context.h"
#include "Scene.h"
#include "GameObject.h"

#include "Components/MoveComponent.h"
#include "Components/SpriteComponent.h"


Engine::Engine() : 
	_fixedDeltaTime(1.f / 60.f),
	_deltaTime(0.f),
	_sceneManager(Context(
		_window, 
		_lua)
	)
{
	LuaInit_Base(_lua);
	_window.create(sf::VideoMode(600, 600), "Test");
}


void Engine::Run()
{
	float timeSinceLastFrame = 0.f;

	_textures.Load("Eagle", "media/textures/Eagle.png");

	Scene* scene = new Scene(_sceneManager.GetContext());
	std::unique_ptr<GameObject> obj(new GameObject());
	auto& sprite = obj->AddComponent<SpriteComponent, false, false, true>();
	auto& move = obj->AddComponent<MoveComponent, false, true, false>();
	sprite.SetTexture(_textures["Eagle"]);
	move.SetSpeed(100);
	obj->Start();
	scene->SetRoot(std::move(obj));
	_sceneManager.PushScene(scene);

	_clock.restart();
	while (_window.isOpen())
	{
		_deltaTime = _clock.restart().asSeconds();
		timeSinceLastFrame += _deltaTime;
		HandleEvents();
		Update();
		while (timeSinceLastFrame > _fixedDeltaTime)
		{
			timeSinceLastFrame -= _fixedDeltaTime;
			FixedUpdate();
		}
		Render();
	}
}


void Engine::HandleEvents()
{
	sf::Event ev;
	while (_window.pollEvent(ev))
	{
		switch (ev.type)
		{
			case sf::Event::Closed:
			_window.close();
			break;
		}
	}
}


void Engine::Update()
{
	_sceneManager.Update(_deltaTime);
}

void Engine::FixedUpdate()
{
	_sceneManager.FixedUpdate(_fixedDeltaTime);
}

void Engine::Render()
{
	_window.clear();
	_sceneManager.Render();
	_window.display();
}