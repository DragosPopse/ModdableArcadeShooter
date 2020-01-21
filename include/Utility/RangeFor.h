#pragma once

/*
	Utility class that converts a container into a reverse container to be used in a range-based for loop
	Basically, it swaps begin/end with rbegin/rend
*/

template <class Iterable>
class ReverseFor
{
	Iterable& _iterable;

public:
	explicit ReverseFor(Iterable& iterable) :
		_iterable(iterable)
	{
	}

	auto begin() { return _iterable.rbegin(); }
	auto end() { return _iterable.rend(); }
};