#include "Engine.h"

#include <iostream>
#include <ctime>

#include "Scene.h"
#include "Scenes/Level.h"
#include "Scenes/LevelLoader.h"
#include "Scenes/SplashScreen.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"

#include "LuaInit.h"
#include "Context.h"
#include "GameObject.h"
#include "Random.h"



Engine::Engine() : 
	_fixedDeltaTime(1.f / 60.f),
	_deltaTime(0.f),
	_sceneManager(&_context)
{
	srand(static_cast<unsigned int>(time(static_cast<time_t>(0))));
	LuaInit_Base(_lua);
	LuaInit_SFML(_lua);
	LuaInit_Thor(_lua);
	LuaInit_Game(_lua);
	_window.create(sf::VideoMode(800, 800), "Bitflight", sf::Style::Close);
	SeedLua(_lua);
	

	_context.lua = &_lua;
	_context.music = &_music;
	_context.player = &_player;
	_context.window = &_window;
}


void Engine::Run()
{
	float timeSinceLastFrame = 0.f;
		
	_theme.load("assets/textures/DefaultTheme.txt");

	tgui::Theme::setDefault(&_theme);

	//std::shared_ptr<LevelLoader> entry(new LevelLoader(_sceneManager.GetContext(), "assets/scripts/levels/Level1.lua"));
	std::shared_ptr<SplashScreen> entry(new SplashScreen(_sceneManager.GetContext()));
	_sceneManager.PushScene(entry);

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

		if (_sceneManager.IsEmpty())
		{
			_window.close();
		}
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