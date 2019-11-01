#include "Context.h"


Context::Context(
	sf::RenderWindow* w,
	sol::state* l,
	sf::Music* m,
	Player* p
) :
	window(w),
	lua(l),
	music(m),
	player(p)
{
}