#include <iostream>

#include "Engine.h"
#include "LuaInit.h"
#include "Context.h"
#include "Scene.h"
#include "GameObject.h"

#include "Scenes/Level.h"


Engine::Engine() : 
	_fixedDeltaTime(1.f / 60.f),
	_deltaTime(0.f),
	_context(
		&_window,
		&_lua
	),
	_sceneManager(&_context)
{
	LuaInit_Base(_lua);
	_window.create(sf::VideoMode(600, 600), "Test");
}


void Engine::Run()
{
	float timeSinceLastFrame = 0.f;

	_textures.Load("Eagle", "assets/textures/Eagle.png");


	_sceneManager.PushScene(new Level(_sceneManager.GetContext(), "Level1.lua"));

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