#pragma once

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