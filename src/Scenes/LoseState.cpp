#include "Scenes/LoseState.h"
#include "Scenes/Level.h"
#include "Context.h"
#include "Utility.h"
#include <iostream>
#include "Scenes/MainMenu.h"

#include <fstream>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace rjs = rapidjson;

LoseState::LoseState(Context* context, Level* level) :
	Scene(context),
	_level(level),
	_elapsedTime(0.f),
	_backgroundAnimation(1, 0),
	_backgroundFading(true),
	_animationDuration(3.f),
	_textAnimation(1.f, 0.f),
	_timePerIncrement(2.f),
	_animationFinalScale(1.5f),
	_scaleAnimationDuration(0.2f),
	_currentScore(0)
{
	_background.setFillColor(sf::Color(0, 0, 0, 0));
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));

	_yourScore.setFont(_level->GetDefaultFont());
	_score.setFont(_level->GetDefaultFont());
	_scoreInfo.setFont(_level->GetDefaultFont());
	_highScoreText.setFont(_level->GetDefaultFont());
	_highScore.setFont(_level->GetDefaultFont());
	_yourScore.setCharacterSize(30);
	_score.setCharacterSize(30);
	_scoreInfo.setCharacterSize(30);
	_highScoreText.setCharacterSize(30);
	_highScore.setCharacterSize(30);
	_yourScore.setString("Your Score: ");
	_highScoreText.setString(BuildString("High Score: ", std::to_string(_level->GetHighScore())));
	_score.setString("0");

	_yourScore.setPosition(0, _context->window->getSize().y / 2 - _yourScore.getCharacterSize());
	AdaptScorePosition();
	_highScoreText.setPosition(0, _yourScore.getPosition().y + _highScoreText.getCharacterSize() * 2);

	for (auto i = _level->_enemiesDowned.cbegin(); i != _level->_enemiesDowned.cend(); ++i)
	{
		_increments[BuildString(i->first, " Downed")] = i->second * _level->_airplaneDataDict[i->first].score;
	}

	_currentIncrement = _increments.cbegin();
}

//Sorry about this awfully written code over here. Turns out I still don't know how to do pretty animations also look pretty in code
bool LoseState::Update(float dt)
{
	if (_currentIncrement != _increments.cend())
	{
		if (_backgroundFading)
		{
			_elapsedTime += dt;
			float progress = _elapsedTime / _animationDuration;

			if (progress <= 1.f)
			{
				_backgroundAnimation(_background, progress);
				_textAnimation(_yourScore, progress);
				_textAnimation(_score, progress);
				_context->music->setVolume(Lerp(_context->player->GetMusicVolume(), 0.f, progress));
			}
			else
			{
				_elapsedTime = 0.f;
				_backgroundFading = false;
				_playingIncrementAnimation = true;
				_nextIncrementScore = _currentIncrement->second;
			}
		}
		else if (_playingIncrementAnimation)
		{
			_elapsedTime += dt;
			float progress = _elapsedTime / _timePerIncrement;
			if (progress <= 1.f)
			{
				int nextScore = Lerp(_currentScore, _nextIncrementScore, progress);
				_score.setString(std::to_string(nextScore));
			}
			else
			{
				_elapsedTime = 0.f;
				_currentScore = _nextIncrementScore;
				_score.setString(std::to_string(_currentScore));
				_playingIncrementAnimation = false;
				_playingScaleAnimation = true;
			}
		}
		else if (_playingScaleAnimation)
		{
			_elapsedTime += dt;
			float progress = _elapsedTime / _scaleAnimationDuration;
			if (progress <= 1.f)
			{
				float scale = Lerp(1.f, _animationFinalScale, progress);
				_score.setScale(scale, scale);
				CenterOrigin(_score);
			}
			else
			{
				_elapsedTime = 0.f;
				_score.setScale(_animationFinalScale, _animationFinalScale);
				CenterOrigin(_score);
				_playingScaleAnimation = false;
				_playingScaleAnimationReverse = true;
			}
		}
		else if (_playingScaleAnimationReverse)
		{
			_elapsedTime += dt;
			float progress = _elapsedTime / _scaleAnimationDuration;
			if (progress <= 1.f)
			{
				float scale = Lerp(_animationFinalScale, 1.f, progress);
				_score.setScale(scale, scale);
				CenterOrigin(_score);
			}
			else
			{
				_elapsedTime = 0.f;
				_score.setScale(1.f, 1.f);
				CenterOrigin(_score);
				++_currentIncrement;
				_playingScaleAnimationReverse = false;
				_playingIncrementAnimation = true;
			}
		}
	}
	else
	{
		
	}
	

	AdaptScorePosition();

	return true;
}


bool LoseState::HandleEvent(const sf::Event& ev)
{
	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}
	else if (ev.type == sf::Event::KeyPressed)
	{
		if (ev.key.code == sf::Keyboard::Space)
		{
			if (!_backgroundFading)
			{
				if (_currentIncrement == _increments.cend())
				{
					RequestClear();
					std::shared_ptr<MainMenu> menu(new MainMenu(_context, false));
					RequestPush(menu);
					if (_currentScore)
					{
						std::ofstream out(_level->_saveFile);
						rjs::OStreamWrapper wrapper(out);
						rjs::Document document;
						document.SetObject();
						auto& allocator = document.GetAllocator();
						document.AddMember("HighScore", _currentScore, allocator);
						rjs::PrettyWriter writer(wrapper);
						document.Accept(writer);  
					}
				}
			}
		}
	}
	return false;
}


bool LoseState::FixedUpdate(float dt)
{
	return true;
}


bool LoseState::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_context->window->draw(_background);
	_context->window->draw(_yourScore);
	_context->window->draw(_score);
	_context->window->draw(_scoreInfo);
	_context->window->draw(_skipInfo);
	_context->window->draw(_highScoreText);
	return false;
}


void LoseState::AdaptScorePosition()
{
	//_score.setPosition(_context->window->getSize().x - _score.getCharacterSize() * _score.getString().getSize(), _context->window->getSize().y / 2 - _score.getCharacterSize() / 2);
	_score.setPosition(_context->window->getSize().x / 2, _context->window->getSize().y / 2 - _score.getCharacterSize() / 2);
	CenterOrigin(_score);
}