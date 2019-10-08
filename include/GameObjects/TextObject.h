#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>


class TextObject :
	public GameObject
{
	sf::Text _text;

public:
	TextObject();

	void SetFont(const sf::Font& font);

	void SetString(const std::string& str);

	void SetCharSize(unsigned int size);

	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};