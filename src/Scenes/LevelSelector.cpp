#include "Scenes/LevelSelector.h"

#include <filesystem>
#include <fstream>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <sol/sol.hpp>


#include "Scenes/MainMenu.h"
#include "Scenes/LevelLoader.h"
#include "Scenes/Level.h"

#include "Context.h"
#include "Utility/Sol.h"
#include "Utility/String.h"
#include "Utility/Math.h"


#define LEVELS_PATH "assets/scripts/levels/"


namespace rjs = rapidjson;


LevelSelector::LevelSelector(Context* context, MainMenu* menu) :
	Scene(context),
	_menu(menu),
	_currentIndex(0)
{
	_background.setFillColor(sf::Color(0, 0, 0, 100));
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(static_cast<float>(_context->window->getSize().x), static_cast<float>(_context->window->getSize().y)));
	_levelTitle.setFont(_menu->_fonts["Menu"]);
	_levelScore.setFont(_menu->_fonts["Menu"]);
	_levelTitle.setCharacterSize(40u);
	_levelScore.setCharacterSize(30u);
	_infoText.setCharacterSize(20u);
	_infoText.setFont(_menu->_fonts["Menu"]);

	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(LEVELS_PATH))
	{
		bool good = true;
		std::string file = entry.path().string();
		sol::table level;
		std::string title;
		std::string previewImage;
		std::string saveFile;
		int highScore = 0;
		try
		{
			level = Protect<sol::table>(_context->lua->do_file(file));
			title = Protect<std::string>(level["title"]);
			previewImage = Protect<std::string>(level["previewImage"]);
			saveFile = Protect<std::string>(level["saveFile"]);
		}
		catch (sol::error&)
		{
			good = false;
		}
		if (good)
		{
			std::ifstream in(saveFile);
			if (in.good())
			{
				rjs::IStreamWrapper wrapper(in);
				rjs::Document document;
				document.ParseStream(wrapper);
				highScore = document["HighScore"].GetInt();
			}
			
		}
		else
		{
			title = file;
			previewImage = "assets/textures/levels/Level1.png";
		}
		_levelData.emplace_back(file, title, previewImage, highScore, good);

		_textures.Load(title, previewImage);
	}

	UpdateDisplay();
}


void LevelSelector::Enable()
{
	_infoText.setString(_context->player->Parse("Press {MoveLeft}/{MoveRight} to change level, \n{Fire} to confirm and {Exit} to go back."));
	_leftKey = _context->player->GetKey(Player::MoveLeft);
	_rightKey = _context->player->GetKey(Player::MoveRight);
	_fireKey = _context->player->GetKey(Player::Fire);
	_exitKey = _context->player->GetKey(Player::Exit);
	CenterOrigin(_infoText);
}


bool LevelSelector::HandleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		RequestClear();
		break;

	case sf::Event::KeyPressed:
		if (ev.key.code == _exitKey)
		{
			RequestPop();
			_menu->SetVisible(true);
		}
		else if (ev.key.code == _fireKey)
		{
			RequestClear();
			std::shared_ptr<LevelLoader> loader(new LevelLoader(_context, _levelData[_currentIndex].file));
			RequestPush(loader);
		}
		else if (ev.key.code == _leftKey)
		{
			_currentIndex--;
			if (_currentIndex < 0)
			{
				_currentIndex = _levelData.size() - 1;
			}
			UpdateDisplay();
		}
		else if (ev.key.code == _rightKey)
		{
			_currentIndex++;
			if (_currentIndex >= static_cast<int>(_levelData.size()))
			{
				_currentIndex = 0;
			}
			UpdateDisplay();
		}
		break; 
	}
	return false;
}


bool LevelSelector::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_context->window->draw(_background);
	_context->window->draw(_levelImage);
	_context->window->draw(_levelTitle);
	_context->window->draw(_infoText);
	_context->window->draw(_levelScore);
	return true;
}


bool LevelSelector::Update(float)
{
	return true;
}


void LevelSelector::UpdateDisplay()
{
	auto& level = _levelData[_currentIndex];
	_levelImage.setTexture(_textures[level.title]);
	_levelTitle.setString(level.title);
	if (!level.good)
	{
		_levelScore.setString("Lua Error");
	}
	else if (level.highScore != 0)
	{
		_levelScore.setString(BuildString("High Score: ", level.highScore));
	}
	else
	{
		_levelScore.setString("No High Score Set");

	}
	_levelImage.setScale(3, 3);

	CenterOrigin(_levelTitle);
	CenterOrigin(_levelImage);
	CenterOrigin(_levelScore);
	_levelImage.setPosition(static_cast<float>(_context->window->getSize().x) / 2.f, static_cast<float>(_context->window->getSize().y) / 2.f);
	_levelScore.setPosition(static_cast<float>(_context->window->getSize().x) / 2.f,
		static_cast<float>(_context->window->getSize().y) / 2.f - _levelImage.getGlobalBounds().height / 2.f - static_cast<float>(_levelTitle.getCharacterSize()));
	_levelTitle.setPosition(static_cast<float>(_context->window->getSize().x) / 2.f, static_cast<float>(_levelScore.getPosition().y) - static_cast<float>(_levelScore.getCharacterSize()) / 2.f - static_cast<float>(_levelTitle.getCharacterSize()));
	_infoText.setPosition(_context->window->getSize().x / 2.f, 
		static_cast<float>(_context->window->getSize().y) / 2.f + _levelImage.getGlobalBounds().height / 2.f + static_cast<float>(_infoText.getCharacterSize()));
}