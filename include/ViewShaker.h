#pragma once

#include <SFML/Graphics.hpp>
#include "Utility.h"

class ViewShaker
{
	sf::View* _view;
	
	bool _shaking;
	float _elapsedTime;
	float _amplitude;
	float _duration;
	float _yDisplacement;
	sf::Vector2f _originalPosition;
	std::mt19937 _rng; //should seed this
	UniformCircleDistribution _distribution;

public:
	ViewShaker();

	void SetView(sf::View* view);

	void Update(float dt);

	void Shake(float amplitude, float duration);

	bool IsShaking() const;
};