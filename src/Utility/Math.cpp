#include "Utility/Math.h"

#include <SFML/Graphics.hpp>


void CenterOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}


void CenterOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

}


float Magnitude(const sf::Vector2f& vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}


sf::Vector2f Normalize(const sf::Vector2f& vector)
{
	return vector / Magnitude(vector);
}


float ToDegree(float radian)
{
	return 180.f / M_PI * radian;
}


float ToRadian(float degree)
{
	return M_PI / 180.f * degree;
}


sf::Vector2f Direction(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return Normalize(from - to);
}


float Distance(const sf::Vector2f& rhs, const sf::Vector2f& lhs)
{
	return sqrt((rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y));
}


float Lerp(float start, float end, float progress)
{
	return start + progress * (end - start);
}


sf::Vector2f Lerp(const sf::Vector2f& start, const sf::Vector2f& end, float progress)
{
	return start + progress * (end - start);
}