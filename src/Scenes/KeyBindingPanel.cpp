#include "Scenes/KeyBindingPanel.h"

#include "Utility/String.h"
#include "Scenes/ErrorScene.h"


KeyBindingPanel::KeyBindingPanel(Context* context, GuiMenu* settingsPanel, const tgui::Font& font, const sf::SoundBuffer& clickSound) :
	Scene(context),
	_settingsPanel(settingsPanel),
	_assigningKey(false),
	_textSize(30),
	_gui(*context->window)
{
	_click.setBuffer(clickSound);
	_panel = tgui::VerticalLayout::create();

	_confirmButton = tgui::Button::create("Confirm");

	_moveLeftButton = tgui::Button::create(BuildString("Move Left: ", ToString(_context->player->GetKey(Player::ActionType::MoveLeft))));
	_moveRightButton = tgui::Button::create(BuildString("Move Right: ", ToString(_context->player->GetKey(Player::ActionType::MoveRight))));
	_moveUpButton = tgui::Button::create(BuildString("Move Up: ", ToString(_context->player->GetKey(Player::ActionType::MoveUp))));
	_moveDownButton = tgui::Button::create(BuildString("Move Down: ", ToString(_context->player->GetKey(Player::ActionType::MoveDown))));
	_fireButton = tgui::Button::create(BuildString("Fire: ", ToString(_context->player->GetKey(Player::ActionType::Fire))));
	_nextWeaponButton = tgui::Button::create(BuildString("Next Weapon: ", ToString(_context->player->GetKey(Player::ActionType::NextWeapon))));
	_previousWeaponButton = tgui::Button::create(BuildString("Previous Weapon: ", ToString(_context->player->GetKey(Player::ActionType::PreviousWeapon))));
	_pauseButton = tgui::Button::create(BuildString("Exit/Pause: ", ToString(_context->player->GetKey(Player::ActionType::Exit))));


	_keyButtons.push_back(_moveLeftButton);
	_keyButtons.push_back(_moveRightButton);
	_keyButtons.push_back(_moveUpButton);
	_keyButtons.push_back(_moveDownButton);
	_keyButtons.push_back(_fireButton);
	_keyButtons.push_back(_nextWeaponButton);
	_keyButtons.push_back(_previousWeaponButton);
	_keyButtons.push_back(_pauseButton);

	auto assignKey = [this](tgui::Button::Ptr button, const std::string& text, Player::ActionType action)
	{
		_assigningKey = true;
		_currentKeyButton = button;
		_buttonText = text;
		_currentKeyButton->setText(BuildString(text, ": ..."));
		_confirmButton->setEnabled(false);
		for (int i = 0; i < static_cast<int>(_keyButtons.size()); i++)
		{
			_keyButtons[i]->setEnabled(false);
		}
		_actionToBind = action;
	};

	_moveLeftButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_moveLeftButton, "Move Left", Player::ActionType::MoveLeft); });
	_moveRightButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_moveRightButton, "Move Right", Player::ActionType::MoveRight); });
	_moveUpButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_moveUpButton, "Move Up", Player::ActionType::MoveUp); });
	_moveDownButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_moveDownButton, "Move Down", Player::ActionType::MoveDown); });
	_fireButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_fireButton, "Fire", Player::ActionType::Fire); });
	_previousWeaponButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_previousWeaponButton, "Previous Weapon", Player::ActionType::PreviousWeapon); });
	_nextWeaponButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_nextWeaponButton, "Next Weapon", Player::ActionType::NextWeapon); });
	_pauseButton->connect("pressed", [this, assignKey]() {_click.setVolume(_context->player->GetSfxVolume()); _click.play(); assignKey(_pauseButton, "Exit/Pause", Player::ActionType::Exit); });


	_confirmButton->connect("pressed",
		[this]()
		{
			_click.setVolume(_context->player->GetSfxVolume());
			_click.play(); 
			SetVisible(false);
			_settingsPanel->SetVisible(true);
			RequestPop();
		});

	_panel->setSize("70%");
	_panel->setPosition("(&.size - size) / 2");
	for (int i = 0; i < static_cast<int>(_keyButtons.size()); i++)
	{
		_keyButtons[i]->setTextSize(_textSize);
		_keyButtons[i]->setInheritedFont(font);
		_panel->add(_keyButtons[i]);
		_panel->addSpace(0.1f);
	}
	_confirmButton->setTextSize(_textSize);
	_confirmButton->setInheritedFont(font);
	_panel->add(_confirmButton);

	_gui.add(_panel);
}


void KeyBindingPanel::SetVisible(bool visible)
{
	_panel->setVisible(visible);
}


bool KeyBindingPanel::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);
	if (_assigningKey && ev.type == sf::Event::KeyPressed)
	{
		_assigningKey = false;
		for (int i = 0; i < static_cast<int>(_keyButtons.size()); i++)
		{
			_keyButtons[i]->setEnabled(true);
		}
		_confirmButton->setEnabled(true);

		if (_context->player->Contains(ev.key.code))
		{
			_currentKeyButton->setText(BuildString(_buttonText, ": ", ToString(_context->player->GetKey(_actionToBind))));
			std::shared_ptr<ErrorScene> error(new ErrorScene(_context, "Invalid Key", 
				BuildString(ToString(ev.key.code), " is already assigned to other action. Please choose another key. ")));
			RequestPush(std::move(error));
		}
		else
		{
			_currentKeyButton->setText(BuildString(_buttonText, ": ", ToString(ev.key.code)));
			_context->player->AssignKey(_actionToBind, ev.key.code);
		}
	}

	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}

	return false;
}


bool KeyBindingPanel::Update(float)
{
	return true;
}


bool KeyBindingPanel::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_gui.draw();
	return true;
}