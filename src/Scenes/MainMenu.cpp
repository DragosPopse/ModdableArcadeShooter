#include "Scenes/MainMenu.h"

#include <iostream>
#include <string>

#include <SFML/Audio.hpp>

#include "Scenes/LevelSelector.h"
#include "Scenes/SettingsPanel.h"
#include "Scenes/ErrorScene.h"
#include "Scenes/LevelLoader.h"

#include "Context.h"


MainMenu::MainMenu(Context* context, bool firstTime) :
	Scene(context),
	_gui(*context->window),
	_scrollSpeed(60),
	_firstIndex(1),
	_nextIndex(0),
	_fade(0.9f, 0),
	_elapsedTime(0),
	_fadeDuration(0.5f),
	_textSize(30),
	_firstTime(firstTime)
{
	if (_context->player->HasSettings())
	{
		_context->player->LoadSettings();
	}
	else
	{
		_context->player->SaveSettings();
	}

	_sounds.Load("Click", "assets/audio/sfx/Click.wav");
	_clickSound.setBuffer(_sounds["Click"]);
	_clickSound.setVolume(_context->player->GetSfxVolume());

	_view = _context->window->getDefaultView();
	_textures.Load("Background", "assets/textures/MenuBackground.png");
	_fonts.Load("Menu", "assets/fonts/pcsenior.ttf");

	_settingsPanel.reset(new SettingsPanel(_context, this, _fonts["Menu"], _sounds["Click"]));

	_parts[0].setTexture(_textures["Background"]);
	_parts[1].setTexture(_textures["Background"]);
	float scale = (float)_context->window->getSize().x / _textures["Background"].getSize().x;
	_parts[0].setScale(scale, scale);
	_parts[1].setScale(scale, scale);
	_parts[1].setPosition(0, _parts[0].getPosition().y - _parts[0].getGlobalBounds().height);

	_panel = tgui::VerticalLayout::create();
	_settingsButton = tgui::Button::create("Settings");
	_playButton = tgui::Button::create("Play");
	_creditsButton = tgui::Button::create("Credits");
	_exitButton = tgui::Button::create("Exit");

	_settingsButton->setTextSize(_textSize);
	_playButton->setTextSize(_textSize);
	_creditsButton->setTextSize(_textSize);
	_exitButton->setTextSize(_textSize);

	_playButton->setInheritedFont(_fonts["Menu"]);
	_creditsButton->setInheritedFont(_fonts["Menu"]);
	_exitButton->setInheritedFont(_fonts["Menu"]);
	_settingsButton->setInheritedFont(_fonts["Menu"]);

	_panel->setSize("40%");
	_panel->setPosition("(&.size - size) / 2");
	_panel->add(_playButton);
	_panel->addSpace(0.1f);
	_panel->add(_settingsButton);
	_panel->addSpace(0.1f);
	_panel->add(_creditsButton);
	_panel->addSpace(0.1f);
	_panel->add(_exitButton);

	_exitButton->connect("pressed",
		[this]()
		{
			_clickSound.setVolume(_context->player->GetSfxVolume());
			_clickSound.play();
			RequestClear();
		});

	_playButton->connect("pressed",
		[this]()
		{
			_clickSound.setVolume(_context->player->GetSfxVolume());
			_clickSound.play();
			SetVisible(false);
			RequestPush(_levelSelector);
		});

	_settingsButton->connect("pressed",
		[this]()
		{
			_clickSound.setVolume(_context->player->GetSfxVolume());
			_clickSound.play();
			SetVisible(false);
			_settingsPanel->SetVisible(true);
			RequestPush(_settingsPanel);
		});




	
	
	_gui.add(_panel);

	_levelSelector.reset(new LevelSelector(_context, this));


	SetVisible(true);
}


void MainMenu::Enable()
{
	if (!_firstTime)
	{
		_context->music->openFromFile("assets/audio/music/MainMenu.wav");
		_context->music->setVolume(_context->player->GetMusicVolume());
		_context->music->play();
	}
}



void MainMenu::SetVisible(bool visible)
{
	_panel->setVisible(visible);
}





bool MainMenu::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);

	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}

	else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::X)
	{
		std::shared_ptr<ErrorScene> error(new ErrorScene(_context, "TEST", "MESSAGETEST"));
		RequestPush(std::move(error));
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
	
	return true;
}


void MainMenu::SwitchSprites()
{
	_parts[_nextIndex].setPosition(0, _parts[_firstIndex].getPosition().y - _parts[_firstIndex].getGlobalBounds().height);

	int aux = _firstIndex;
	_firstIndex = _nextIndex;
	_nextIndex = aux;
}