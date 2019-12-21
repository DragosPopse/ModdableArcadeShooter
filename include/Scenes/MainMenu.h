#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include "Player.h"
#include <Thor/Animations.hpp>
#include <memory>
#include "GuiMenu.h"

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
	tgui::Button::Ptr _creditsButton;
	tgui::Button::Ptr _exitButton;
	tgui::VerticalLayout::Ptr _panel;


	std::shared_ptr<LevelSelector> _levelSelector;
	std::shared_ptr<SettingsPanel> _settingsPanel;

	sf::Sound _clickSound;
	size_t _textSize;

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