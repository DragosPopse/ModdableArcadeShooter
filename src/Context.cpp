#include "Context.h"


Context::Context(
	sf::RenderWindow* w,
	sol::state* l,
	sf::Music* m
) :
	window(w),
	lua(l),
	music(m)
{
}