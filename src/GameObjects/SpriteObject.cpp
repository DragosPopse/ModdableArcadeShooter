#include "GameObjects/SpriteObject.h"

#include "Utility.h"


SpriteObject::SpriteObject()
{
}


void SpriteObject::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
	CenterOrigin(_sprite);
}


void SpriteObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}


void SpriteObject::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
	CenterOrigin(_sprite);
}


sf::FloatRect SpriteObject::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(_sprite.getGlobalBounds());
}
