#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include "Player.h"
#include <Thor/Animations.hpp>
#include <memory>
#include "Menu.h"

#include "Scenes/LevelSelector.h"

class MainMenu :
	public Scene,
	public Menu
{
	friend class KeyBindingFunction;
	friend class LevelSelector;

	tgui::Gui _gui;
	sf::View _view;
	float _scrollSpeed;
	sf::Sprite _parts[2];

	int _firstIndex;
	int _nextIndex;

	thor::FadeAnimation _fade;
	float _elapsedTime;
	float _fadeDuration;

	bool _assigningKey;
	tgui::Button::Ptr _currentKeyButton;
	std::string _buttonText;
	Player::ActionType _actionToBind;

	tgui::Button::Ptr _playButton;
	tgui::Button::Ptr _settingsButton;
	tgui::Button::Ptr _creditsButton;
	tgui::Button::Ptr _exitButton;
	tgui::VerticalLayout::Ptr _mainPanel;
	tgui::VerticalLayout::Ptr _settingsPanel;
	tgui::VerticalLayout::Ptr _keyBindingsPanel;

	tgui::Slider::Ptr _musicSlider;
	tgui::Slider::Ptr _sfxSlider;
	tgui::Label::Ptr _musicLabel;
	tgui::Label::Ptr _sfxLabel;
	tgui::Button::Ptr _keyBindingsButton;

	tgui::Button::Ptr _moveLeftButton;
	tgui::Button::Ptr _moveRightButton;
	tgui::Button::Ptr _moveUpButton;
	tgui::Button::Ptr _moveDownButton;
	tgui::Button::Ptr _fireButton;
	tgui::Button::Ptr _nextWeaponButton;
	tgui::Button::Ptr _previousWeaponButton;

	std::vector<tgui::Button::Ptr> _keyButtons;

	tgui::Button::Ptr _confirmSettingsButton;
	tgui::Button::Ptr _confirmBindingsButton;

	int _mainTextSize;
	int _settingsTextSize;
	int _keybindingTextSize;

	std::shared_ptr<LevelSelector> _levelSelector;

	sf::Sound _clickSound;

public:
	MainMenu(Context* context, bool firstTime = false);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;

	void SetVisible(bool visible) override;

private:
	void SwitchSprites();

	void EnableSettings();
	void EnableMain();
	void EnableKeyBindings();
	void DisableAll();
	
	void SetupMain();
	void SetupSettings();
	void SetupKeyBindings();
	void SetupCredits();
	void SetupSelector();
};