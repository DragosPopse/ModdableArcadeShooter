#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class SpriteComponent :
	public Component
{
	sf::Sprite _sprite;
public:
	void Init() override;

	void Start() override;

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite& GetSprite();

	void SetTexture(const sf::Texture& texture);
};