#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

#define SCRIPTS_PATH "assets/scripts/"
#define TEXTURES_PATH "assets/textures/"
#define AIRPLANES_PATH "assets/scripts/airplanes/"
#define LEVELS_PATH "assets/scripts/levels/"
#define PROJECTILES_PATH "assets/scripts/projectiles/"

void CenterOrigin(sf::Sprite& sprite);
void CenterOrigin(sf::Text& text);

float Magnitude(const sf::Vector2f vector);
sf::Vector2f Normalize(const sf::Vector2f& vector);

template <class... Args>
std::string BuildString(const Args& ... args)
{
	std::ostringstream stream;
	using List= int[];
	(void)List {
		0, ((void)(stream << args), 0) ...
	};

	return stream.str();
}


float ToRadian(float degree);
float ToDegree(float radian);