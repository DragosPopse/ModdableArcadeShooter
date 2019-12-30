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
	void SetTextureRect(const sf::IntRect& rect);
	void SetColor(const sf::Color& color);
	const sf::Color& GetColor() const;

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::FloatRect GetBoundingRect() const override;
};