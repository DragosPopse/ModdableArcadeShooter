#pragma once

/*
	Wrapper for sf::Text
*/

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

	void SetCharSize(unsigned int size);
	unsigned int GetCharSize() const;

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetColor(const sf::Color& color);

	const sf::Color& GetColor() const;

	unsigned int GetCategory() const override { return static_cast<unsigned int>(Type::Text); }
};