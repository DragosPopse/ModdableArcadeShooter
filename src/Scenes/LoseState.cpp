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
	_backgroundAnimation(1.f, 0.f, 
		[](sf::RectangleShape& animated, const sf::Color& color)
		{
			animated.setFillColor(color);
		},
		[](const sf::RectangleShape& animated)
		{
			return animated.getFillColor();
		}),
	_backgroundFading(true),
	_animationDuration(3.f),
	_textAnimation(1.f, 0.f, 
		[](TextObject& animated, const sf::Color& color)
		{
			animated.SetColor(color);
		},
		[](const TextObject& animated)
		{
			return animated.GetColor();
		}),
	_flickerAnimation(0.1f, 0.1f, 
		[](TextObject& animated, const sf::Color& color)
		{
			animated.SetColor(color);
		},
		[](const TextObject& animated)
		{
			return animated.GetColor();
		}),
	_yourScore(new TextObject()),
	_score(new NumberIncrementAnimation()),
	_incrementDuration(2.5f),
	_scaleDuration(0.2f),
	_charSize(30u),
	_finalCharSize(50u),
	_highScoreAnimation([](TextObject& animated, size_t size)
	{
			animated.SetCharSize(size);
	}),
	_highScore(new TextObject()),
	_highScoreText(new TextObject()),
	_flickerElapsedTime(0.f),
	_flickerDuration(2.f),
	_infoText(new TextObject()),
	_summaryText(new TextAnimation("")),
	_letterDuration(0.06f),
	_currentIncrement(-1)
{
	_background.setFillColor(sf::Color(0, 0, 0, 0));
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));

	_summaryText->SetTimePerLetter(_letterDuration);
	_highScoreAnimation.SetBeginCharSize(_charSize);
	_highScoreAnimation.SetFinalCharSize(_finalCharSize);
	
	std::unique_ptr<NumberIncrementAnimation> scorePtr(_score);
	std::unique_ptr<TextObject> highScorePtr(_highScore);
	_highScoreText->AddChild(std::move(highScorePtr));
	_yourScore->AddChild(std::move(scorePtr));

	_summaryText->SetFont(_level->GetDefaultFontID()); //Inconsistency in design
	_summaryText->Start(_level); //Inconsistency in design
	_summaryText->SetCharSize(15);
	_summaryText->setPosition(_context->window->getSize().x / 2, _charSize);

	_yourScore->SetFont(_level->GetDefaultFont());
	_score->SetFont(_level->GetDefaultFont());
	_highScoreText->SetFont(_level->GetDefaultFont());
	_highScore->SetFont(_level->GetDefaultFont());
	_infoText->SetFont(_level->GetDefaultFont());
	_yourScore->SetCharSize(_charSize);
	_highScore->SetCharSize(_charSize);
	_highScoreText->SetCharSize(_charSize);
	_infoText->SetCharSize(_charSize);
	_score->SetCharSize(_charSize);
	_yourScore->SetString("Your Score:");
	_highScoreText->SetString("High Score:");
	_infoText->SetString(_context->player->Parse("Press {Fire} to Skip"));
	_highScore->SetString(std::to_string(_level->_highScore));
	
	_score->SetFinalCharSize(_finalCharSize);
	_score->SetIncrementDuration(_incrementDuration);
	_score->SetScaleDuration(_scaleDuration);
	_yourScore->setPosition(_context->window->getSize().x / 2, _context->window->getSize().y / 2);
	_score->setPosition(0.f, _yourScore->GetCharSize() / 2 + _score->GetCharSize());
	_highScoreText->setPosition(_score->GetWorldPosition().x, _score->GetWorldPosition().y + _score->GetCharSize() * 2);
	_highScore->setPosition(0.f, _highScoreText->GetCharSize() / 2 + _highScore->GetCharSize());
	_infoText->setPosition(_context->window->getSize().x / 2, _context->window->getSize().y - _infoText->GetCharSize());
	 
	int currentScore = 0;
	for (auto it = _level->_enemiesDowned.cbegin(); it != _level->_enemiesDowned.cend(); ++it)
	{
		if (it->second != 0)
		{
			currentScore += _level->_airplaneDataDict[it->first].score * it->second;
			_score->AddIncrement(currentScore);
			if (it->second == 1)
			{
				_summaries.push_back(BuildString("You Downed ", it->second, " ", it->first, " Airplane"));
			}
			else
			{
				_summaries.push_back(BuildString("You Downed ", it->second, " ", it->first, " Airplanes"));
			}
		}
	}
}
	 

bool LoseState::Update(float dt)
{
	if (_backgroundFading)
	{
		_elapsedTime += dt;
		float progress = _elapsedTime / _animationDuration;

		if (progress <= 1.f)
		{
			_backgroundAnimation(_background, progress);
			_textAnimation(*_yourScore, progress);
			_textAnimation(*_score, progress);
			_textAnimation(*_highScore, progress);
			_textAnimation(*_highScoreText, progress);
			_context->music->setVolume(Lerp(_context->player->GetMusicVolume(), 0.f, progress));
		}
		else
		{
			_score->Start(this);
			_backgroundFading = false;
			_elapsedTime = 0.f;
		}
	}
	else
	{
		_score->Update(dt);
		if (_summaries.size() != 0 && 
			_currentIncrement != _score->GetCurrentIncrement() 
			&& _score->GetCurrentState() != NumberIncrementAnimation::StateID::None)
		{
			_currentIncrement = _score->GetCurrentIncrement();
			_summaryText->SetFinalString(_summaries[_currentIncrement]);
		}
		_summaryText->Update(dt);
		if (_score->GetCurrentState() == NumberIncrementAnimation::StateID::Scale 
			&& _score->GetCurrentNumber() > _level->_highScore)
		{
			_elapsedTime += dt;
			float progress = _elapsedTime / (_scaleDuration * 2.f);
			std::string number = std::to_string(_score->GetCurrentNumber());
			_highScore->SetString(number);
			_highScoreAnimation(*_highScore, progress);
		} 
		else
		{
			_elapsedTime = 0.f;
		}

		if (_score->GetCurrentState() == NumberIncrementAnimation::StateID::None)
		{
			_infoText->SetString(_context->player->Parse("Press {Fire} to Continue"));
		}
	}

	_flickerElapsedTime += dt;
	float progress = _flickerElapsedTime / _flickerDuration;
	if (progress > 1.f)
	{
		_flickerElapsedTime = 0.f;
	}
	else
	{
		_flickerAnimation(*_infoText, progress);
	}

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
		if (ev.key.code == _context->player->GetKey(Player::Fire))
		{
			if (!_backgroundFading)
			{
				if (_score->GetCurrentState() != NumberIncrementAnimation::StateID::None)
				{
					_score->Skip();
					_summaryText->Skip();
				}
				else
				{
					RequestClear();
					std::shared_ptr<MainMenu> menu(new MainMenu(_context, false));
					RequestPush(menu);
					if (_score->GetCurrentNumber() > _level->_highScore)
					{
						std::ofstream out(_level->_saveFile);
						rjs::OStreamWrapper wrapper(out);
						rjs::Document document;
						document.SetObject();
						auto& allocator = document.GetAllocator();
						document.AddMember("HighScore", _score->GetCurrentNumber(), allocator);
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
	_yourScore->Draw(*_context->window, sf::RenderStates::Default);
	_highScoreText->Draw(*_context->window, sf::RenderStates::Default);
	if (!_backgroundFading)
	{
		_infoText->Draw(*_context->window, sf::RenderStates::Default);
	}
	_summaryText->Draw(*_context->window, sf::RenderStates::Default);
	//_score->Draw(*_context->window, sf::RenderStates::Default);
	return false;
}