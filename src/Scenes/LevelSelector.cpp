#include "Scenes/LevelSelector.h"
#include "Scenes/MainMenu.h"
#include <filesystem>
#include "Utility.h"
#include <sol/sol.hpp>
#include "Context.h"
#include "Scenes/LevelLoader.h"
#include "Scenes/Level.h"

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream>

namespace rjs = rapidjson;

LevelSelector::LevelSelector(Context* context, MainMenu* menu) :
	Scene(context),
	_menu(menu),
	_currentIndex(0)
{
	_background.setFillColor(sf::Color(0, 0, 0, 100));
	_background.setPosition(0, 0);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));
	_levelTitle.setFont(_menu->_fonts["Menu"]);
	_levelScore.setFont(_menu->_fonts["Menu"]);
	_levelTitle.setCharacterSize(40);
	_levelScore.setCharacterSize(30);
	_infoText.setCharacterSize(20);
	_infoText.setFont(_menu->_fonts["Menu"]);
	_infoText.setString("Press A/D to change level, \nSpace to confirm and Escape to go back.");
	CenterOrigin(_infoText);

	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(LEVELS_PATH))
	{
		std::string file = entry.path().string();
		sol::table level = _context->lua->do_file(file);
		std::string title = level["title"];
		std::string previewImage = level["previewImage"];
		std::string saveFile = level["saveFile"];
		int highScore = 0;
		std::ifstream in(saveFile);
		if (in.good())
		{
			rjs::IStreamWrapper wrapper(in);
			rjs::Document document;
			document.ParseStream(wrapper);
			highScore = document["HighScore"].GetInt();
		}
		_levelData.emplace_back(file, title, previewImage, highScore);

		_textures.Load(title, previewImage);
	}

	UpdateDisplay();
}


bool LevelSelector::HandleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		RequestClear();
		break;

	case sf::Event::KeyPressed:
		switch (ev.key.code)
		{
		case sf::Keyboard::Escape:
			RequestPop();
			_menu->EnableMain();
			break;

		case sf::Keyboard::A:
			_currentIndex--;
			if (_currentIndex < 0)
			{
				_currentIndex = _levelData.size() - 1;
			}
			UpdateDisplay();
			break;

		case sf::Keyboard::D:
			_currentIndex++;
			if (_currentIndex >= _levelData.size())
			{
				_currentIndex = 0;
			}
			UpdateDisplay();
			break;

		case sf::Keyboard::Space:
			RequestClear();
			std::shared_ptr<LevelLoader> loader(new LevelLoader(_context, _levelData[_currentIndex].file));
			RequestPush(loader);
			break;
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


bool LevelSelector::Update(float dt)
{
	return true;
}


void LevelSelector::UpdateDisplay()
{
	auto& level = _levelData[_currentIndex];
	_levelImage.setTexture(_textures[level.title]);
	_levelTitle.setString(level.title);
	if (level.highScore != 0)
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
	_levelImage.setPosition(_context->window->getSize().x / 2, _context->window->getSize().y / 2);
	_levelScore.setPosition(_context->window->getSize().x / 2,
		_context->window->getSize().y / 2 - _levelImage.getGlobalBounds().height / 2 - _levelTitle.getCharacterSize());
	_levelTitle.setPosition(_context->window->getSize().x / 2, _levelScore.getPosition().y - _levelScore.getCharacterSize() / 2 - _levelTitle.getCharacterSize());
	_infoText.setPosition(_context->window->getSize().x / 2, 
		_context->window->getSize().y / 2 + _levelImage.getGlobalBounds().height / 2 + _infoText.getCharacterSize());
}