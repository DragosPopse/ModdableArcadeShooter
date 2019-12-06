#include "Scenes/KeyBindingPanel.h"
#include "Utility.h"


KeyBindingPanel::KeyBindingPanel(Context* context, Menu* settingsPanel, const tgui::Font& font, const sf::SoundBuffer& clickSound) :
	Scene(context),
	_settingsPanel(settingsPanel),
	_assigningKey(false),
	_textSize(30),
	_gui(*context->window)
{
	_click.setBuffer(clickSound);
	_panel = tgui::VerticalLayout::create();

	_confirmButton = tgui::Button::create("Confirm");

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
		_confirmButton->setEnabled(false);
		for (int i = 0; i < _keyButtons.size(); i++)
		{
			_keyButtons[i]->setEnabled(false);
		}
		_actionToBind = action;
	};

	_moveLeftButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_moveLeftButton, "Move Left", Player::MoveLeft); });
	_moveRightButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_moveRightButton, "Move Right", Player::MoveRight); });
	_moveUpButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_moveUpButton, "Move Up", Player::MoveUp); });
	_moveDownButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_moveDownButton, "Move Down", Player::MoveDown); });
	_fireButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_fireButton, "Fire", Player::Fire); });
	_previousWeaponButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_previousWeaponButton, "Previous Weapon", Player::PreviousWeapon); });
	_nextWeaponButton->connect("pressed", [this, assignKey]() {_click.play(); assignKey(_nextWeaponButton, "Next Weapon", Player::NextWeapon); });



	_confirmButton->connect("pressed",
		[this]()
		{
			_click.play(); 
			SetVisible(false);
			_settingsPanel->SetVisible(true);
			RequestPop();
		});

	_panel->setSize("70%");
	_panel->setPosition("(&.size - size) / 2");
	for (int i = 0; i < _keyButtons.size(); i++)
	{
		_keyButtons[i]->setTextSize(_textSize);
		_keyButtons[i]->setInheritedFont(font);
		_panel->add(_keyButtons[i]);
		_panel->addSpace(0.1);
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
		_currentKeyButton->setText(BuildString(_buttonText, ": ", ToString(ev.key.code)));
		for (int i = 0; i < _keyButtons.size(); i++)
		{
			_keyButtons[i]->setEnabled(true);
		}
		_confirmButton->setEnabled(true);
		_context->player->AssignKey(_actionToBind, ev.key.code);
	}

	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}

	return false;
}


bool KeyBindingPanel::Update(float dt)
{
	return true;
}


bool KeyBindingPanel::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_gui.draw();
	return true;
}