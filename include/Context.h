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


/*
	A Context is an object shared across all scenes.
*/
struct Context
{
	sf::RenderWindow* window;
	sol::state* lua;
	sf::Music* music;
	Player* player;
};