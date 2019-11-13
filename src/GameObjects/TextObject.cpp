#include "GameObjects/TextObject.h"
#include "Utility.h"
#include <iostream>


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
}


void TextObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_text, states);

	GameObject::Draw(target, states);

}