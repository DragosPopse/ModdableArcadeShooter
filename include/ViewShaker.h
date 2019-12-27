#pragma once

#include <SFML/Graphics.hpp>

#include "Random.h"

class ViewShaker
{
	sf::View* _view;
	
	bool _shaking;
	float _elapsedTime;
	float _amplitude;
	float _duration;
	UniformCircleDistribution _distribution;

public:
	ViewShaker();

	void SetView(sf::View* view);

	void Update(float dt);

	void Shake(float amplitude, float duration);

	bool IsShaking() const;
};