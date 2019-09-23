#include "Context.h"


Context::Context(
	sf::RenderWindow& w,
	sol::state& l
) :
	window(w),
	lua(l)
{
}