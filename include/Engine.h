#pragma once

#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>

class Engine
{
	sf::RenderWindow _window;
	sol::state _lua;
	sf::Clock _clock;
	float _deltaTime;
	float _fixedDeltaTime;

public:
	Engine();

	void Run();

private:
	void HandleEvents();

	void Update();

	void FixedUpdate();

	void Render();
};