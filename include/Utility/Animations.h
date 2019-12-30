#pragma once

#include <functional>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <Thor/Particles/Particle.hpp>

#include "Utility/Math.h"

//This is the same as thor::FadeAnimation but it also generalizes the alpha changing function.
//Use case: sf::RectangleShape uses getFillColor() and setFillColor() for alpha changing, TextObject uses SetColor and GetColor, sf::Sprite uses setColor and getColor
//credits: https://github.com/Bromeon/Thor/blob/master/include/Thor/Animations/FadeAnimation.hpp
template <class Animated>
class GenericFadeAnimation
{
	float _inRatio;
	float _outRatio;
	std::function<void(Animated&, const sf::Color&)> _setColor;
	std::function<sf::Color(const Animated&)> _getColor;
	int _highestAlpha;
	int _lowestAlpha;

public:
	GenericFadeAnimation(float in, float out,
		std::function<void(Animated&, const sf::Color&)> setColor,
		std::function<sf::Color(const Animated&)> getColor) :
		_inRatio(in),
		_outRatio(out),
		_setColor(setColor),
		_getColor(getColor),
		_highestAlpha(255),
		_lowestAlpha(0)
	{
	}


	void SetHighestAlpha(int alpha)
	{
		_highestAlpha = alpha;
	}


	void SetLowestAlpha(int alpha)
	{
		_lowestAlpha = alpha;
	}


	void operator()(Animated& target, float progress) const
	{
		sf::Color color = _getColor(target);

		if (progress < _inRatio)
		{
			color.a = static_cast<sf::Uint8>(Lerp(static_cast<float>(_lowestAlpha), static_cast<float>(_highestAlpha), progress / _inRatio));
		}
		else if (progress > 1.f - _outRatio)
		{
			color.a = static_cast<sf::Uint8>(Lerp(static_cast<float>(_lowestAlpha), static_cast<float>(_highestAlpha), (1.f - progress) / _outRatio));
		}

		_setColor(target, color);
	}
};


template <class Animated>
class TextCharScaleAnimation
{
	std::function<void(Animated&, unsigned int)> _setCharSize;
	unsigned int _finalCharSize;
	unsigned int _beginCharSize;
	bool _upscaling;
	bool _loop;

public:
	TextCharScaleAnimation(std::function<void(Animated&, unsigned int)> setCharSize) :
		_setCharSize(setCharSize),
		_upscaling(true),
		_loop(false),
		_beginCharSize(0),
		_finalCharSize(0)
	{
	}


	void SetLoop(bool loop)
	{
		_loop = loop;
	}


	void SetFinalCharSize(unsigned int size)
	{
		_finalCharSize = size;
	}


	void SetBeginCharSize(unsigned int size)
	{
		_beginCharSize = size;
	}


	void Reset()
	{
		_upscaling = true;
	}


	void operator()(Animated& animated, float progress)
	{
		int currentSize = 0;
		if (_upscaling)
		{
			if (progress <= 0.5f)
			{
				currentSize = static_cast<int>(Lerp(static_cast<float>(_beginCharSize), static_cast<float>(_finalCharSize), progress));
			}
			else
			{
				currentSize = _finalCharSize;
				_upscaling = false;
			}
		}
		else
		{
			if (progress <= 1.f)
			{
				currentSize = static_cast<int>(Lerp(static_cast<float>(_finalCharSize), static_cast<float>(_beginCharSize), progress));
			}
			else
			{
				currentSize = static_cast<int>(_beginCharSize);
				if (_loop)
				{
					Reset();
				}
			}
		}

		_setCharSize(animated, currentSize);
	}
};


using ParticleAnimationFunction = std::function<void(thor::Particle&, sf::Time)>;