#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>	


class SpriteObject :
	public GameObject
{
	sf::Sprite _sprite;

public:
	SpriteObject();

	void SetTexture(const sf::Texture& texture);

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};