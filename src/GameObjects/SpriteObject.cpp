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