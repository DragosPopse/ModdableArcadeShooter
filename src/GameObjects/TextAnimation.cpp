#include "GameObjects/TextAnimation.h"

#include <iostream>

#include "Scenes/Level.h"
#include "Utility/Math.h"


TextAnimation::TextAnimation(const std::string& finalStr) :
	_finalStr(finalStr),
	_elapsedTime(0),
	_destroyOnFinish(false),
	_finished(false),
	_currentLetter(0)
{
	SetString(_currentStr.str());
	_strSize = static_cast<int>(_finalStr.size());
}


void TextAnimation::SetFinalString(const std::string& finalStr)
{
	_currentStr.str(std::string());
	_currentStr.clear();
	_finalStr = finalStr;
	_finished = false;
	_currentLetter = 0;
	_elapsedTime = 0.f;
	_strSize = static_cast<int>(_finalStr.size());
}


void TextAnimation::Skip()
{
	_currentStr.str(_finalStr);
	SetString(_finalStr);
	_finished = true;
}


void TextAnimation::SetString(const std::string& str)
{
	_text.setString(str);
	//std::cout << _text.getString().toAnsiString() << '\n';
	CenterOrigin(_text);
}


void TextAnimation::Start(Scene* scene)
{
	Level* level = static_cast<Level*>(scene);
	auto& fonts = level->GetFonts();
	_text.setFont(fonts[_font]);
}

 
void TextAnimation::SetFont(const std::string& font)
{
	_font = font;
}


void TextAnimation::Update(float dt)
{
	_elapsedTime += dt;
	if (_finished && _destroyOnFinish)
	{
		if (_elapsedTime > _timeBeforeDestroy)
		{
			MarkForDestroy();
		}
	}
	else if (!_finished)
	{
		if (_elapsedTime > _timePerLetter)
		{
			_elapsedTime = 0;
			if (_currentLetter == _strSize)
			{
				_finished = true;
			}
			else
			{
				while (_finalStr[_currentLetter] == ' ')
				{
					_currentStr << _finalStr[_currentLetter++];
				}
				_currentStr << _finalStr[_currentLetter];
				SetString(_currentStr.str());
				_currentLetter++;
			}
		}
	}
}


void TextAnimation::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_text, states);

	GameObject::Draw(target, states);
}


bool TextAnimation::IsFinished() const
{
	return _finished;
}


void TextAnimation::SetTimePerLetter(float time)
{
	_timePerLetter = time;
}


void TextAnimation::SetDestroyOnFinish(float timeBeforeDestroy)
{
	_destroyOnFinish = true;
	_timeBeforeDestroy = timeBeforeDestroy;
}


void TextAnimation::SetCharSize(unsigned int n)
{
	_text.setCharacterSize(n);
}


void TextAnimation::SetColor(const sf::Color& color)
{
	_text.setFillColor(color);
}