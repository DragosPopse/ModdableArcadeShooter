#pragma once

#include "Scene.h"
#include "GuiMenu.h"

#include <memory>

#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>

class KeyBindingPanel;

class SettingsPanel :
	public Scene,
	public GuiMenu
{
	tgui::VerticalLayout::Ptr _panel;

	tgui::Slider::Ptr _musicSlider;
	tgui::Slider::Ptr _sfxSlider;
	tgui::Label::Ptr _musicLabel;
	tgui::Label::Ptr _sfxLabel;
	tgui::Button::Ptr _keyBindingsButton;

	tgui::Button::Ptr _confirmSettingsButton;

	GuiMenu* _keyBindingsPanel;
	GuiMenu* _mainPanel;

	tgui::Gui _gui;

	sf::Sound _click;

	size_t _textSize;

	std::shared_ptr<KeyBindingPanel> _keyBindingPanel;

public:
	SettingsPanel(Context* context, GuiMenu* mainPanel, const tgui::Font& font, const sf::SoundBuffer& clickSound);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool FixedUpdate(float dt) override;
	bool Render() override;

	void SetVisible(bool visible) override;
};