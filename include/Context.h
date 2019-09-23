#pragma once

namespace sol
{
	class state;
}

namespace sf
{
	class RenderWindow;
}

struct Context
{
	sf::RenderWindow& window;
	sol::state& lua;

	Context(
		sf::RenderWindow& w,
		sol::state& l
	);
};