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

class Player;

struct Context
{
	sf::RenderWindow* window;
	sol::state* lua;
	sf::Music* music;
	Player* player;

	Context(
		sf::RenderWindow* w,
		sol::state* l,
		sf::Music* m,
		Player* p
	);
};