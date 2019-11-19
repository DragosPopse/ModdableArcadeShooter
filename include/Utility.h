#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include <string>
#include <sstream>
#include <random>
#include <functional>
#include <Thor/Animations.hpp>
#include <Thor/Particles.hpp>

#define SCRIPTS_PATH "assets/scripts/"
#define TEXTURES_PATH "assets/textures/"
#define AIRPLANES_PATH "assets/scripts/airplanes/"
#define LEVELS_PATH "assets/scripts/levels/"
#define PROJECTILES_PATH "assets/scripts/projectiles/"
#define CONFIG_PATH "assets/config/"
#define FONTS_PATH "assets/fonts/"
#define PICKUPS_PATH "assets/scripts/pickups/"

void CenterOrigin(sf::Sprite& sprite);
void CenterOrigin(sf::Text& text);

float Magnitude(const sf::Vector2f vector);
sf::Vector2f Normalize(const sf::Vector2f& vector);
sf::Vector2f Direction(const sf::Vector2f& from, const sf::Vector2f& to);
float Distance(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
float Lerp(float start, float end, float progress);

std::string ToString(sf::Keyboard::Key key);

///from https://stackoverflow.com/questions/21806561/concatenating-strings-and-numbers-in-variadic-template-function
template <class... Args>
std::string BuildString(const Args& ... args)
{
	std::ostringstream stream;
	using List= int[];
	(void)List {
		0, ((void)(stream << args), 0) ...
	};

	return stream.str();
}


float ToRadian(float degree);
float ToDegree(float radian);


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


class ParticleFrameAnimation
{
public:
	ParticleFrameAnimation(int firstIndex, int lastIndex);

};


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

	
public:
	GenericFadeAnimation(float in, float out, 
		std::function<void(Animated&, const sf::Color&)> setColor, 
		std::function<sf::Color(const Animated&)> getColor) :
		_inRatio(in),
		_outRatio(out),
		_setColor(setColor),
		_getColor(getColor)
	{
	}
	

	void operator()(Animated& target, float progress) const
	{
		sf::Color color = _getColor(target);

		if (progress < _inRatio)
		{
			color.a = 256 * progress / _inRatio;
		}
		else if (progress > 1.f - _outRatio)
		{
			color.a = 256 * (1.f - progress) / _outRatio;
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


using ParticleAnimationFunction = std::function<void(thor::Particle&, sf::Time)> ;