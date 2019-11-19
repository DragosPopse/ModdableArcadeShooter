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
	_yourScore(new TextObject()),
	_score(new NumberIncrementAnimation())
{
	_background.setFillColor(sf::Color(0, 0, 0, 0));
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));
	
	std::unique_ptr<NumberIncrementAnimation> scorePtr(_score);
	_yourScore->AddChild(std::move(scorePtr));

	_yourScore->SetFont(_level->GetDefaultFont());
	_score->SetFont(_level->GetDefaultFont());
	_yourScore->SetCharSize(30u);
	_score->SetCharSize(30u);
	_yourScore->SetString("Your Score:");
	
	_score->SetFinalCharSize(50u);
	_score->SetIncrementDuration(2.5f);
	_score->SetScaleDuration(0.2f);
	_yourScore->setPosition(_context->window->getSize().x / 2, _context->window->getSize().y / 2);
	_score->setPosition(0.f, _yourScore->GetCharSize() / 2 + _score->GetCharSize());
	
	 

	for (auto it = _level->_enemiesDowned.cbegin(); it != _level->_enemiesDowned.cend(); ++it)
	{
		_score->AddIncrement(it->second * _level->_airplaneDataDict[it->first].score);
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
			_context->music->setVolume(Lerp(_context->player->GetMusicVolume(), 0.f, progress));
		}
		else
		{
			_score->Start(this);
			_backgroundFading = false;
		}
	}
	else
	{
		_score->Update(dt);
		
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
		if (ev.key.code == sf::Keyboard::Space)
		{
			if (!_backgroundFading)
			{
				if (_score->GetCurrentState() != NumberIncrementAnimation::StateID::None)
				{
					_score->Skip();
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
	//_score->Draw(*_context->window, sf::RenderStates::Default);
	return false;
}