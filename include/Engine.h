#pragma once

/*
	Contains code for initializing the game, the Lua state and window.
	Contains the game loop.
*/

#include <memory>

#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>

#include "SceneManager.h"
#include "ResourceHolder.h"

#include "Player.h"


class Engine
{
	sf::RenderWindow _window;
	sol::state _lua;
	sf::Clock _clock;
	float _deltaTime;
	float _fixedDeltaTime;
	SceneManager _sceneManager;
	TextureHolder _textures;
	
	Context _context;

	tgui::Theme _theme;

	sf::Music _music;

	Player _player;

public:
	Engine();

	void Run();

private:
	void HandleEvents();

	void Update();

	void FixedUpdate();

	void Render();
};