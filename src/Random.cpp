#include "Random.h"

#include <random>

#include <sol/state.hpp>

static thread_local std::random_device s_rd;
thread_local std::mt19937 g_RNG(s_rd());


void SeedLua(sol::state& lua)
{
	lua["_cpp_seed"] = s_rd();
	lua.do_string("math.randomseed(_cpp_seed)");
	lua["_cpp_seed"] = sol::nil;
}