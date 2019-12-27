#pragma once

#include <sol/sol.hpp>

template <class ReturnType, class ParamType>
ReturnType Protect(ParamType result)
{
	if (!result.valid())
	{
		sol::error err = result;
		throw err;
	}
	return result;
}

template<class ParamType>
void Protect(ParamType result)
{
	if (!result.valid())
	{
		sol::error err = result;
		throw err;
	}
}