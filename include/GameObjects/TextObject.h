#pragma once

#include "GameObject.h"

#include <string>

#include <SFML/Graphics.hpp>


class TextObject :
	public GameObject
{
	sf::Text _text;

public:
	TextObject();

	void SetFont(const sf::Font& font);

	void SetString(const sf::String& str);

	void SetCharSize(size_t size);
	size_t GetCharSize() const;

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetColor(const sf::Color& color);

	sf::Color GetColor() const;

	unsigned int GetCategory() const override { return TextType; }
};