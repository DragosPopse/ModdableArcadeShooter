#pragma once

#include <random>
#include <SFML/Graphics.hpp>

class Emitter
{
	sf::Vector2f _position;
	float _emissionRate;

public:
	void SetParticlesPerSeocond(int particles);
};