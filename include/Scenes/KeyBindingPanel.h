#pragma once

#include "Scene.h"
#include "GuiMenu.h"
#include "Player.h"

#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include <string>


class KeyBindingPanel :
	public Scene,
	public GuiMenu
{
	tgui::VerticalLayout::Ptr _panel;

	tgui::Button::Ptr _moveLeftButton;
	tgui::Button::Ptr _moveRightButton;
	tgui::Button::Ptr _moveUpButton;
	tgui::Button::Ptr _moveDownButton;
	tgui::Button::Ptr _fireButton;
	tgui::Button::Ptr _nextWeaponButton;
	tgui::Button::Ptr _previousWeaponButton;
	tgui::Button::Ptr _pauseButton;

	std::vector<tgui::Button::Ptr> _keyButtons;

	tgui::Button::Ptr _confirmButton;
	GuiMenu* _settingsPanel;

	tgui::Button::Ptr _currentKeyButton;
	bool _assigningKey;
	Player::ActionType _actionToBind;
	std::string _buttonText;

	sf::Sound _click;

	size_t _textSize;

	tgui::Gui _gui;

public:
	KeyBindingPanel(Context* context, GuiMenu* settingsPanel, const tgui::Font& font, const sf::SoundBuffer& clickSound);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;

	void SetVisible(bool visible) override;
};