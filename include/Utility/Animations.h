#pragma once

#include <functional>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <Thor/Particles/Particle.hpp>

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
	int _defaultAlpha;

public:
	GenericFadeAnimation(float in, float out,
		std::function<void(Animated&, const sf::Color&)> setColor,
		std::function<sf::Color(const Animated&)> getColor) :
		_inRatio(in),
		_outRatio(out),
		_setColor(setColor),
		_getColor(getColor),
		_defaultAlpha(255)
	{
	}


	void SetHighestAlpha(int alpha)
	{
		_defaultAlpha = alpha;
	}


	void operator()(Animated& target, float progress) const
	{
		sf::Color color = _getColor(target);

		if (progress < _inRatio)
		{
			color.a = (_defaultAlpha + 1) * progress / _inRatio;
		}
		else if (progress > 1.f - _outRatio)
		{
			color.a = (_defaultAlpha + 1) * (1.f - progress) / _outRatio;
		}

		_setColor(target, color);
	}
};


template <class Animated>
class TextCharScaleAnimation
{
	std::function<void(Animated&, size_t)> _setCharSize;
	size_t _finalCharSize;
	size_t _beginCharSize;
	bool _upscaling;
	bool _loop;

public:
	TextCharScaleAnimation(std::function<void(Animated&, size_t)> setCharSize) :
		_setCharSize(setCharSize),
		_upscaling(true),
		_loop(false)
	{
	}


	void SetLoop(bool loop)
	{
		_loop = loop;
	}


	void SetFinalCharSize(size_t size)
	{
		_finalCharSize = size;
	}


	void SetBeginCharSize(size_t size)
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
				currentSize = Lerp(_beginCharSize, _finalCharSize, progress);
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
				currentSize = Lerp(_finalCharSize, _beginCharSize, progress);
			}
			else
			{
				currentSize = _beginCharSize;
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