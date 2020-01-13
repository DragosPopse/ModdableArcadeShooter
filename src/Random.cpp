#include "Random.h"

#include <random>

#include <sol/state.hpp>
#include <SFML/Graphics.hpp>

static thread_local std::random_device s_rd;
thread_local std::mt19937 RNG;


void SeedLua(sol::state& lua)
{
	lua["_cpp_seed"] = s_rd();
	auto result = lua.do_string(std::string("math.randomseed(_cpp_seed)"));
	lua["_cpp_seed"] = sol::nil;
}


UniformVector2fDistribution::UniformVector2fDistribution(float min, float max) :
	_distr(min, max)
{
}


sf::Vector2f UniformVector2fDistribution::operator()()
{
	float n = _distr(RNG);
	return sf::Vector2f(n, n);
}