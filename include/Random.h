#pragma once

#include <random>

#include <SFML/System/Vector2.hpp>
#include <Thor/Math/Distribution.hpp>

#include "Utility/Math.h"


namespace sol
{
	class state;
}

extern thread_local std::mt19937 RNG;

template <class RealType, class GeneratorType = std::mt19937>
RealType RandReal(RealType min, RealType max)
{
	static thread_local std::uniform_real_distribution<RealType> distribution;
	return distribution(RNG, decltype(distribution)::param_type(min, max));
}


template <class IntegerType, class GeneratorType = std::mt19937>
IntegerType RandInt(IntegerType min, IntegerType max)
{
	static thread_local std::uniform_int_distribution<IntegerType> distribution;
	return distribution(RNG, decltype(distribution)::param_type(min, max));
}


void SeedLua(sol::state& lua);


//This converts to a thor::Distribution 
class UniformVector2fDistribution
{
	std::uniform_real_distribution<float> _distr;
public:
	UniformVector2fDistribution(float min, float max);

	sf::Vector2f operator()();

	static thor::Distribution<sf::Vector2f> Create(float min, float max)
	{
		UniformVector2fDistribution distr(min, max);
		return thor::Distribution<sf::Vector2f>(distr);
	}
};



/*
	Random utility that returns a Vector2f inside a circle of radius 1
	https://programming.guide/random-point-within-circle.html
*/
class UniformCircleDistribution
{
	std::uniform_real_distribution<float> _distribution;

public:
	UniformCircleDistribution() :
		_distribution(0.f, 1.f)
	{
	}

	template <class Generator>
	sf::Vector2f operator()(Generator& generator)
	{
		float theta = _distribution(generator) * 2.f * PIf;
		float distance = sqrt(_distribution(generator));

		float x = distance * cos(theta);
		float y = distance * sin(theta);

		return { x, y };
	}
};

/*
	Returns points inside an annulus. 
	This can be used for particles to specify a Circle Distribution that also has a minimum speed
	https://stackoverflow.com/questions/13064912/generate-a-uniformly-random-point-within-an-annulus-ring
*/
class AnnulusDistribution
{
	std::uniform_real_distribution<float> _distribution;
	float _innerRadius;
	float _outerRadius;

public:
	AnnulusDistribution(float innerRadius, float outerRadius) :
		_distribution(0.f, 1.f),
		_innerRadius(innerRadius),
		_outerRadius(outerRadius)
	{
	}

	template <class Generator>
	sf::Vector2f operator()(Generator generator)
	{
		float theta = 2.f * PIf * _distribution(generator);
		float distance = sqrt(_distribution(generator) * (_innerRadius * _innerRadius - _outerRadius * _outerRadius) + _outerRadius * _outerRadius);

		float x = distance * cos(theta);
		float y = distance * sin(theta);
		return { x, y };
	}

	sf::Vector2f operator()()
	{
		return operator()(RNG);
	}

	static thor::Distribution<sf::Vector2f> Create(float innerRadius, float outerRadius)
	{
		return AnnulusDistribution(innerRadius, outerRadius);
	}
};
