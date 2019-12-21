#pragma once

#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include "SceneManager.h"
#include "ResourceHolder.h"
#include "Player.h"
#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Engine
{
	sf::RenderWindow _window;
	std::unique_ptr<sol::state> _lua;
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

	void ResetLua();

private:
	void HandleEvents();

	void Update();

	void FixedUpdate();

	void Render();
};