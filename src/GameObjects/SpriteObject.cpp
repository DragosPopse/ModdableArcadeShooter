#include "GameObjects/SpriteObject.h"


SpriteObject::SpriteObject()
{
}


void SpriteObject::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
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
}


sf::FloatRect SpriteObject::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(_sprite.getGlobalBounds());
}
