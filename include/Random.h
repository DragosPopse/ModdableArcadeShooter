#pragma once

#include <random>

#include <SFML/System/Vector2.hpp>
#include <Thor/Math/Distribution.hpp>


namespace sol
{
	class state;
}

extern thread_local std::mt19937 g_RNG;

template <class RealType, class GeneratorType = std::mt19937>
RealType RandReal(RealType min, RealType max)
{
	static thread_local std::uniform_real_distribution<RealType> distribution;
	return distribution(g_RNG, decltype(distribution)::param_type(min, max));
}


template <class IntegerType, class GeneratorType = std::mt19937>
IntegerType RandInt(IntegerType min, IntegerType max)
{
	static thread_local std::uniform_int_distribution<IntegerType> distribution;
	return distribution(g_RNG, decltype(distribution)::param_type(min, max));
}


void SeedLua(sol::state& lua);


//This converts to a thor::Distribution 
class UniformVector2fDistribution
{
	std::mt19937 _generator;
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
		float a = _distribution(generator) * 2.f * M_PI;
		float r = sqrt(_distribution(generator));

		float x = r * cos(a);
		float y = r * sin(a);

		return { x, y };
	}
};
