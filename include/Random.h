#pragma once

#include <random>

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