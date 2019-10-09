#pragma once

#include <SFML/Graphics.hpp>

struct Particle
{
	Particle() :
		position(0, 0),
		color(255, 255, 255),
		lifetime(0),
		velocity(0, 0)
	{
	}

	sf::Vector2f position;
	sf::Color color;
	float lifetime;
	sf::Vector2f velocity;
};