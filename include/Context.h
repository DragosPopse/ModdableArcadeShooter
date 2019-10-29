#pragma once

namespace sol
{
	class state;
}

namespace sf
{
	class RenderWindow;
	class Music;
}

struct Context
{
	sf::RenderWindow* window;
	sol::state* lua;
	sf::Music* music;

	Context(
		sf::RenderWindow* w,
		sol::state* l,
		sf::Music* m
	);
};