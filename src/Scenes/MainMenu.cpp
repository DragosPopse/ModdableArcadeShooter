#include "Scenes/MainMenu.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include "Scenes/LevelLoader.h"
#include <string>
#include "Utility.h"
#include "Scenes/LevelSelector.h"
#include "Context.h"


MainMenu::MainMenu(Context* context, bool firstTime) :
	Scene(context),
	_gui(*context->window),
	_scrollSpeed(60),
	_firstIndex(1),
	_nextIndex(0),
	_fade(0.9, 0),
	_elapsedTime(0),
	_fadeDuration(0.5f),
	_assigningKey(false),
	_mainTextSize(30),
	_keybindingTextSize(30),
	_settingsTextSize(30)
{
	if (_context->player->HasSettings())
	{
		_context->player->LoadSettings();
	}
	else
	{
		_context->player->SaveSettings();
	}

	_mainPanel = tgui::VerticalLayout::create();
	_settingsPanel = tgui::VerticalLayout::create();
	_keyBindingsPanel = tgui::VerticalLayout::create();
	_mainPanel->setVisible(true);
	_settingsPanel->setVisible(false);
	_keyBindingsPanel->setVisible(false);
	
	
	SetupMain();
	SetupSettings();
	SetupKeyBindings();
	SetupCredits();
	SetupSelector();
	
	_gui.add(_settingsPanel);
	_gui.add(_mainPanel);
	_gui.add(_keyBindingsPanel);

	_levelSelector.reset(new LevelSelector(_context, this));
	if (!firstTime)
	{
		_context->music->openFromFile("assets/audio/music/MainMenu.wav");
		_context->music->setVolume(_context->player->GetMusicVolume());
		_context->music->play();
	}
}


void MainMenu::SetupMain()
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

	_settingsButton = tgui::Button::create("Settings");
	_playButton = tgui::Button::create("Play");
	_creditsButton = tgui::Button::create("Credits");
	_exitButton = tgui::Button::create("Exit");

	_settingsButton->setTextSize(_mainTextSize);
	_playButton->setTextSize(_mainTextSize);
	_creditsButton->setTextSize(_mainTextSize);
	_exitButton->setTextSize(_mainTextSize);

	_playButton->setInheritedFont(_fonts["Menu"]);
	_creditsButton->setInheritedFont(_fonts["Menu"]);
	_exitButton->setInheritedFont(_fonts["Menu"]);
	_settingsButton->setInheritedFont(_fonts["Menu"]);

	_mainPanel->setSize("40%");
	_mainPanel->setPosition("(&.size - size) / 2");
	_mainPanel->add(_playButton);
	_mainPanel->addSpace(0.1);
	_mainPanel->add(_settingsButton);
	_mainPanel->addSpace(0.1);
	_mainPanel->add(_creditsButton);
	_mainPanel->addSpace(0.1);
	_mainPanel->add(_exitButton);

	_exitButton->connect("pressed",
		[this]()
		{
			RequestClear();
		});

	_playButton->connect("pressed",
		[this]()
		{
			//RequestClear();
			//std::shared_ptr<LevelLoader> level(new LevelLoader(_context, "Level1.lua"));
			//RequestPush(level);
			DisableAll();
			RequestPush(_levelSelector);
		});

	_settingsButton->connect("pressed",
		[this]()
		{
			EnableSettings();
		});

	
}


void MainMenu::EnableMain()
{
	DisableAll();
	_mainPanel->setVisible(true);
}


void MainMenu::EnableSettings()
{
	DisableAll();
	_settingsPanel->setVisible(true);
}


void MainMenu::EnableKeyBindings()
{
	DisableAll();
	_keyBindingsPanel->setVisible(true);
}


void MainMenu::DisableAll()
{
	_settingsPanel->setVisible(false);
	_mainPanel->setVisible(false);
	_keyBindingsPanel->setVisible(false);
}


void MainMenu::SetupSelector()
{

}


void MainMenu::SetupCredits()
{

}


void MainMenu::SetupSettings()
{
	_musicSlider = tgui::Slider::create(0, 100);
	_sfxSlider = tgui::Slider::create(0, 100);
	_musicLabel = tgui::Label::create("Music");
	_sfxLabel = tgui::Label::create("SFX");
	_confirmSettingsButton = tgui::Button::create("Confirm");
	_keyBindingsButton = tgui::Button::create("Key Bindings"); 

	_confirmSettingsButton->connect("pressed", 
		[this]()
		{
			EnableMain();
			_context->player->SaveSettings();
		});
	
	_keyBindingsButton->connect("pressed",
		[this]()
		{
			EnableKeyBindings();
		});

	_sfxSlider->setValue(_context->player->GetSfxVolume());
	_musicSlider->setValue(_context->player->GetMusicVolume());

	_sfxSlider->connect("ValueChanged", [this](float v)
		{
			_context->player->SetSfxVolume(v);
		});
	_musicSlider->connect("ValueChanged", [this](float v)
		{
			_context->player->SetMusicVolume(v);
			_context->music->setVolume(v);
		});

	_settingsPanel->setSize("60%");
	_settingsPanel->setPosition("(&.size - size) / 2");
	_sfxLabel->getRenderer()->setTextColor(sf::Color::Black);
	_musicLabel->getRenderer()->setTextColor(sf::Color::Black);
	//_sfxLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	//_musicLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	_sfxLabel->getRenderer()->setFont(_fonts["Menu"]);
	_musicLabel->getRenderer()->setFont(_fonts["Menu"]);
	_sfxLabel->setTextSize(_settingsTextSize);
	_musicLabel->setTextSize(_settingsTextSize);
	_keyBindingsButton->setTextSize(_settingsTextSize);
	_confirmSettingsButton->setTextSize(_settingsTextSize);
	_confirmSettingsButton->setInheritedFont(_fonts["Menu"]);
	_keyBindingsButton->setInheritedFont(_fonts["Menu"]);
	_keyBindingsButton->setTextSize(_settingsTextSize);


	_settingsPanel->add(_sfxLabel);
	_settingsPanel->add(_sfxSlider);
	_settingsPanel->add(_musicLabel);
	_settingsPanel->add(_musicSlider);
	_settingsPanel->addSpace(0.1);
	_settingsPanel->add(_keyBindingsButton);
	_settingsPanel->addSpace(0.1);
	_settingsPanel->add(_confirmSettingsButton);
}







void MainMenu::SetupKeyBindings()
{
	_confirmBindingsButton = tgui::Button::create("Confirm");

	_moveLeftButton = tgui::Button::create(BuildString("Move Left: ", ToString(_context->player->GetKey(Player::MoveLeft))));
	_moveRightButton = tgui::Button::create(BuildString("Move Right: ", ToString(_context->player->GetKey(Player::MoveRight))));
	_moveUpButton = tgui::Button::create(BuildString("Move Up: ", ToString(_context->player->GetKey(Player::MoveUp))));
	_moveDownButton = tgui::Button::create(BuildString("Move Down: ", ToString(_context->player->GetKey(Player::MoveDown))));
	_fireButton = tgui::Button::create(BuildString("Fire: ", ToString(_context->player->GetKey(Player::Fire))));
	_nextWeaponButton = tgui::Button::create(BuildString("Next Weapon: ", ToString(_context->player->GetKey(Player::NextWeapon))));
	_previousWeaponButton = tgui::Button::create(BuildString("Previous Weapon: ", ToString(_context->player->GetKey(Player::PreviousWeapon))));
	


	_keyButtons.push_back(_moveLeftButton);
	_keyButtons.push_back(_moveRightButton);
	_keyButtons.push_back(_moveUpButton);
	_keyButtons.push_back(_moveDownButton);
	_keyButtons.push_back(_fireButton);
	_keyButtons.push_back(_nextWeaponButton);
	_keyButtons.push_back(_previousWeaponButton);

	auto assignKey = [this](tgui::Button::Ptr button, const std::string& text, Player::ActionType action)
	{
		_assigningKey = true;
		_currentKeyButton = button;
		_buttonText = text;
		_currentKeyButton->setText(BuildString(text, ": ..."));
		_confirmBindingsButton->setEnabled(false);
		for (int i = 0; i < _keyButtons.size(); i++)
		{
			_keyButtons[i]->setEnabled(false);
		}
		_actionToBind = action;
	};

	_moveLeftButton->connect("pressed", [this, assignKey]() {assignKey(_moveLeftButton, "Move Left", Player::MoveLeft); });
	_moveRightButton->connect("pressed", [this, assignKey]() {assignKey(_moveRightButton, "Move Right", Player::MoveRight); });
	_moveUpButton->connect("pressed", [this, assignKey]() {assignKey(_moveUpButton, "Move Up", Player::MoveUp); });
	_moveDownButton->connect("pressed", [this, assignKey]() {assignKey(_moveDownButton, "Move Down", Player::MoveDown); });
	_fireButton->connect("pressed", [this, assignKey]() {assignKey(_fireButton, "Fire", Player::Fire); });
	_previousWeaponButton->connect("pressed", [this, assignKey]() {assignKey(_previousWeaponButton, "Previous Weapon", Player::PreviousWeapon); });
	_nextWeaponButton->connect("pressed", [this, assignKey]() {assignKey(_nextWeaponButton, "Next Weapon", Player::NextWeapon); });
	


	_confirmBindingsButton->connect("pressed",
		[this]()
		{
			EnableSettings();
		});

	_keyBindingsPanel->setSize("70%");
	_keyBindingsPanel->setPosition("(&.size - size) / 2");
	for (int i = 0; i < _keyButtons.size(); i++)
	{		
		_keyButtons[i]->setTextSize(_keybindingTextSize);
		_keyButtons[i]->setInheritedFont(_fonts["Menu"]);
		_keyBindingsPanel->add(_keyButtons[i]);
		_keyBindingsPanel->addSpace(0.1);
	}
	_confirmBindingsButton->setTextSize(_keybindingTextSize);
	_confirmBindingsButton->setInheritedFont(_fonts["Menu"]);
	_keyBindingsPanel->add(_confirmBindingsButton);
}


bool MainMenu::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);

	if (_assigningKey && ev.type == sf::Event::KeyPressed)
	{
		_assigningKey = false;
		_currentKeyButton->setText(BuildString(_buttonText, ": ", ToString(ev.key.code)));
		for (int i = 0; i < _keyButtons.size(); i++)
		{
			_keyButtons[i]->setEnabled(true);
		}
		_confirmBindingsButton->setEnabled(true);
		_context->player->AssignKey(_actionToBind, ev.key.code);
	}

	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}

	/*if (ev.type == sf::Event::KeyPressed)
	{
		if (ev.key.code == sf::Keyboard::A)
		{
			_context->music->setPitch(_context->music->getPitch() - 0.1);
		}
		else if (ev.key.code == sf::Keyboard::D)
		{
			_context->music->setPitch(_context->music->getPitch() + 0.1);
		}
		std::cout << _context->music->getPitch() << '\n';
	}*/
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