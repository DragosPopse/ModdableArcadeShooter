#include <iostream>

#include "Engine.h"
#include "LuaInit.h"
#include "Context.h"
#include "Scene.h"
#include "GameObject.h"

#include "Scenes/Level.h"
#include "Scenes/LevelLoader.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"

#include <ctime>


namespace
{
	void TestLua(sol::state& lua)
	{
		std::cout << "=====LUA TEST=====\n";
		lua.do_file("assets/scripts/test.lua");
		AirplaneData data;
		data.hitpoints = 200;
		Airplane *plane = new Airplane(&data);
		lua["ptrTest"](plane);
		std::cout << plane->GetHealth();
		delete plane;
		std::cout << "=====TEST END=====\n";
	}
}

Engine::Engine() : 
	_fixedDeltaTime(1.f / 60.f),
	_deltaTime(0.f),
	_context(
		&_window,
		&_lua
	),
	_sceneManager(&_context)
{
	srand(time(0));
	LuaInit_Base(_lua);
	LuaInit_SFML(_lua);
	LuaInit_Game(_lua);
	_window.create(sf::VideoMode(800, 800), "Test");

	TestLua(_lua);
}


void Engine::Run()
{
	float timeSinceLastFrame = 0.f;

	_textures.Load("Eagle", "assets/textures/Eagle.png");


	_sceneManager.PushScene(new LevelLoader(_sceneManager.GetContext(), "Level1.lua"));

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
		_sceneManager.HandleEvent(ev);
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