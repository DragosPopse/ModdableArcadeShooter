#pragma once

/*
	String utilities
*/

#include <string>
#include <sstream>
#include <SFML/Window/Keyboard.hpp>

std::string ToString(sf::Keyboard::Key key);

///from https://stackoverflow.com/questions/21806561/concatenating-strings-and-numbers-in-variadic-template-function
template <class... Args>
std::string BuildString(const Args& ... args)
{
	std::ostringstream stream;
	using List = int[];
	(void)List {
		0, ((void)(stream << args), 0) ...
	};

	return stream.str();
}
