#pragma once

#include <memory>

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
};