#include "Scenes/SettingsPanel.h"

#include "Context.h"
#include "Player.h"

#include "Scenes/KeyBindingPanel.h"


SettingsPanel::SettingsPanel(Context* context, GuiMenu* mainPanel, const tgui::Font& font, const sf::SoundBuffer& clickSound) :
	Scene(context),
	_mainPanel(mainPanel),
	_textSize(30),
	_gui(*context->window),
	_keyBindingPanel(new KeyBindingPanel(context, this, font, clickSound))
{
	_panel = tgui::VerticalLayout::create();
	_click.setBuffer(clickSound);
	_click.setVolume(_context->player->GetSfxVolume());
	_musicSlider = tgui::Slider::create(0, 100);
	_sfxSlider = tgui::Slider::create(0, 100);
	_musicLabel = tgui::Label::create("Music");
	_sfxLabel = tgui::Label::create("SFX");
	_confirmSettingsButton = tgui::Button::create("Confirm");
	_keyBindingsButton = tgui::Button::create("Key Bindings");

	_confirmSettingsButton->connect("pressed",
		[this]()
		{
			_click.play();
			_context->player->SaveSettings();
			SetVisible(false);
			_mainPanel->SetVisible(true);
			RequestPop();
		});

	_keyBindingsButton->connect("pressed",
		[this]()
		{
			_click.play();
			SetVisible(false);
			_keyBindingPanel->SetVisible(true);
			RequestPush(_keyBindingPanel);
		});

	_sfxSlider->setValue(_context->player->GetSfxVolume());
	_musicSlider->setValue(_context->player->GetMusicVolume());

	_sfxSlider->connect("ValueChanged", [this](float v)
		{
			_click.setVolume(_context->player->GetSfxVolume());
			_context->player->SetSfxVolume(v);
		});
	_musicSlider->connect("ValueChanged", [this](float v)
		{
			_context->player->SetMusicVolume(v);
			_context->music->setVolume(v);
		});

	_panel->setSize("60%");
	_panel->setPosition("(&.size - size) / 2");
	_sfxLabel->getRenderer()->setTextColor(sf::Color::Black);
	_musicLabel->getRenderer()->setTextColor(sf::Color::Black);
	//_sfxLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	//_musicLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	_sfxLabel->getRenderer()->setFont(font);
	_musicLabel->getRenderer()->setFont(font);
	_sfxLabel->setTextSize(_textSize);
	_musicLabel->setTextSize(_textSize);
	_keyBindingsButton->setTextSize(_textSize);
	_confirmSettingsButton->setTextSize(_textSize);
	_confirmSettingsButton->setInheritedFont(font);
	_keyBindingsButton->setInheritedFont(font);
	_keyBindingsButton->setTextSize(_textSize);


	_panel->add(_sfxLabel);
	_panel->add(_sfxSlider);
	_panel->add(_musicLabel);
	_panel->add(_musicSlider);
	_panel->addSpace(0.1);
	_panel->add(_keyBindingsButton);
	_panel->addSpace(0.1);
	_panel->add(_confirmSettingsButton);

	_gui.add(_panel);
}


void SettingsPanel::SetVisible(bool visible)
{
	_panel->setVisible(visible);
}


bool SettingsPanel::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);

	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}
	return false;
}


bool SettingsPanel::Update(float dt)
{
	return true;
}


bool SettingsPanel::FixedUpdate(float dt)
{
	return true;
}


bool SettingsPanel::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_gui.draw();
	return true;
}