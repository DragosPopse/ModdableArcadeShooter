#pragma once

#include "Scene.h"
#include "GuiMenu.h"

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <Thor/Animations.hpp>

#include "Player.h"
#include "Scenes/LevelSelector.h"

class SettingsPanel;

class MainMenu :
	public Scene,
	public GuiMenu
{
	friend class KeyBindingFunction;
	friend class LevelSelector;

	tgui::Gui _gui;
	sf::View _view;
	float _scrollSpeed;
	sf::Sprite _parts[2];

	bool _firstTime;

	int _firstIndex;
	int _nextIndex;

	thor::FadeAnimation _fade;
	float _elapsedTime;
	float _fadeDuration;


	tgui::Button::Ptr _playButton;
	tgui::Button::Ptr _settingsButton;
	tgui::Button::Ptr _exitButton;
	tgui::VerticalLayout::Ptr _panel;


	std::shared_ptr<LevelSelector> _levelSelector;
	std::shared_ptr<SettingsPanel> _settingsPanel;

	sf::Sound _clickSound;
	unsigned int _textSize;

public:
	MainMenu(Context* context, bool firstTime = false);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;
	void Enable() override;

	void SetVisible(bool visible) override;

private:
	void SwitchSprites();
};