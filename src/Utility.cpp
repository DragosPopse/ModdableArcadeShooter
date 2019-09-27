#include "Utility.h"
#include <cmath>


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


float Magnitude(const sf::Vector2f vector)
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