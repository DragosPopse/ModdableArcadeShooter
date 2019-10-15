#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include <string>
#include <sstream>
#include <random>

#define SCRIPTS_PATH "assets/scripts/"
#define TEXTURES_PATH "assets/textures/"
#define AIRPLANES_PATH "assets/scripts/airplanes/"
#define LEVELS_PATH "assets/scripts/levels/"
#define PROJECTILES_PATH "assets/scripts/projectiles/"
#define CONFIG_PATH "assets/config/"
#define FONTS_PATH "assets/fonts/"

void CenterOrigin(sf::Sprite& sprite);
void CenterOrigin(sf::Text& text);

float Magnitude(const sf::Vector2f vector);
sf::Vector2f Normalize(const sf::Vector2f& vector);
sf::Vector2f Direction(const sf::Vector2f& from, const sf::Vector2f& to);
float Distance(const sf::Vector2f& lhs, const sf::Vector2f& rhs);


///from https://stackoverflow.com/questions/21806561/concatenating-strings-and-numbers-in-variadic-template-function
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


//This converts to a thor::Distribution 
class UniformVector2fDistribution
{
	std::mt19937 _generator;
	std::uniform_real_distribution<float> _distr;
public:
	UniformVector2fDistribution(float min, float max);

	sf::Vector2f operator()();
};