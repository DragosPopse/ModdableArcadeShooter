#pragma once

#include <cmath>

#include <SFML/System/Vector2.hpp>


namespace sf
{
	class Sprite;
	class Text;
}

void CenterOrigin(sf::Sprite& sprite);
void CenterOrigin(sf::Text& text);

float Magnitude(const sf::Vector2f& vector);
sf::Vector2f Normalize(const sf::Vector2f& vector);
sf::Vector2f Direction(const sf::Vector2f& from, const sf::Vector2f& to);
float Distance(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
float Lerp(float start, float end, float progress);
sf::Vector2f Lerp(const sf::Vector2f& start, const sf::Vector2f& end, float progress);

float ToRadian(float degree);
float ToDegree(float radian);