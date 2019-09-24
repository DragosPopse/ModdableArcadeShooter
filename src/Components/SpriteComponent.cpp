#include <iostream>

#include "Components/SpriteComponent.h"
#include "GameObject.h"


void SpriteComponent::Init()
{

}


void SpriteComponent::Start()
{
}


void SpriteComponent::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//std::cout << _gameObject->GetWorldPosition().x << '\n';

	target.draw(_sprite, states);
}


sf::Sprite& SpriteComponent::GetSprite()
{
	return _sprite;
}


void SpriteComponent::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
}