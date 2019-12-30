#include "GameObjects/TextObject.h"

#include <iostream>

#include "Utility/Math.h"


TextObject::TextObject()
{
}


void TextObject::SetFont(const sf::Font& font)
{
	_text.setFont(font);
}


void TextObject::SetString(const sf::String& str)
{
	_text.setString(str);
	CenterOrigin(_text);
}


void TextObject::SetCharSize(unsigned int size)
{
	_text.setCharacterSize(size);
	CenterOrigin(_text);
}


unsigned int TextObject::GetCharSize() const
{
	return _text.getCharacterSize();
}


void TextObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_text, states);

	GameObject::Draw(target, states);

}


void TextObject::SetColor(const sf::Color& color)
{
	_text.setFillColor(color);
}


const sf::Color& TextObject::GetColor() const
{
	return _text.getFillColor();
}