#pragma once

/*
	When the project was first started I created a different RNG amd distribution for every
	place I required one (It is the first time I am working with RNG is C++). Searching the internet 
	for better ways of doing it I stumbled across the fact that RNG isn't thread-safe in C++ and that you should have 1 RNG
	per thread. I also came across the thread_local keywork and while searching what does it do
	I found this piece of code https://gist.github.com/srivathsanmurali/6ca74bdf154b0f5447321be183ba28d1. I only modified it
	so that it can generate all data types.
*/

#include <random>

template <class RealType, class GeneratorType = std::mt19937>
RealType RandReal(RealType min, RealType max)
{
	static thread_local std::random_device randDevice;
	static thread_local GeneratorType rng(randDevice());
	static thread_local std::uniform_real_distribution<RealType> distribution;
	return distribution(rng, decltype(distribution)::param_type(min, max));
}


template <class IntegerType, class GeneratorType = std::mt19937>
IntegerType RandInt(IntegerType min, IntegerType max)
{
	static thread_local std::random_device randDevice;
	static thread_local GeneratorType rng(randDevice());
	static thread_local std::uniform_real_distribution<IntegerType> distribution;
	return distribution(rng, decltype(distribution)::param_type(min, max));
}