#include "Scenes/MainMenu.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include "Scenes/LevelLoader.h"


MainMenu::MainMenu(Context* context) :
	Scene(context),
	_gui(*context->window),
	_scrollSpeed(60),
	_firstIndex(1),
	_nextIndex(0),
	_fade(0.9, 0),
	_elapsedTime(0),
	_fadeDuration(0.5f)
{
	_view = _context->window->getDefaultView();
	_textures.Load("Background", "assets/textures/MenuBackground.png");
	_fonts.Load("Menu", "assets/fonts/pcsenior.ttf");
	_parts[0].setTexture(_textures["Background"]);
	_parts[1].setTexture(_textures["Background"]);
	float scale = (float)_context->window->getSize().x / _textures["Background"].getSize().x;
	_parts[0].setScale(scale, scale);
	_parts[1].setScale(scale, scale);
	_parts[1].setPosition(0, _parts[0].getPosition().y - _parts[0].getGlobalBounds().height);

	tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
	_settingsButton = tgui::Button::create("Settings");
	_playButton = tgui::Button::create("Play");
	_creditsButton = tgui::Button::create("Credits");
	_exitButton = tgui::Button::create("Exit");
	
	int textSize = 30;
	_settingsButton->setTextSize(textSize);
	_playButton->setTextSize(textSize);
	_creditsButton->setTextSize(textSize);
	_exitButton->setTextSize(textSize);

	_playButton->setInheritedFont(_fonts["Menu"]);
	_creditsButton->setInheritedFont(_fonts["Menu"]);
	_exitButton->setInheritedFont(_fonts["Menu"]);
	_settingsButton->setInheritedFont(_fonts["Menu"]);

	layout->setSize("40%");
	layout->setPosition("(&.size - size) / 2");
	layout->add(_playButton);
	layout->addSpace(0.1);
	layout->add(_settingsButton);
	layout->addSpace(0.1);
	layout->add(_creditsButton);
	layout->addSpace(0.1);
	layout->add(_exitButton);

	_exitButton->connect("pressed", 
		[this]()
		{
			RequestClear();
		});

	_playButton->connect("pressed", 
		[this]()
		{
			RequestClear();
			std::shared_ptr<LevelLoader> level(new LevelLoader(_context, "Level1.lua"));
			RequestPush(level);
		});

	_gui.add(layout);
}


bool MainMenu::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);
	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}
	return false;
}


bool MainMenu::Update(float dt)
{
	_elapsedTime += dt;
	if (_elapsedTime < _fadeDuration)
	{
		_fade(_parts[0], _elapsedTime / _fadeDuration);
		_fade(_parts[1], _elapsedTime / _fadeDuration);
	}
	_view.move(0, -_scrollSpeed * dt);
	if (_view.getCenter().y < _parts[_firstIndex].getPosition().y + _parts[_firstIndex].getGlobalBounds().height / 2)
	{
		
		SwitchSprites();
	}
	return false;
}


bool MainMenu::Render()
{
	_context->window->setView(_view);
	_context->window->draw(_parts[0]);
	_context->window->draw(_parts[1]);
	_context->window->setView(_context->window->getDefaultView());
	_gui.draw();
	
	return false;
}


void MainMenu::SwitchSprites()
{
	_parts[_nextIndex].setPosition(0, _parts[_firstIndex].getPosition().y - _parts[_firstIndex].getGlobalBounds().height);

	int aux = _firstIndex;
	_firstIndex = _nextIndex;
	_nextIndex = aux;
}