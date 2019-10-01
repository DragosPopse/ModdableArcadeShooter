#pragma once

#include <map>
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include "Command.h"

namespace sf
{
	class Event;
}
class CommandQueue;

class Player
{
public:
	enum ActionType
	{
		None,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		NextWeapon,
		PreviousWeapon
	};
private:
	std::map<sf::Keyboard::Key, ActionType> _keyBinding;
	std::map<ActionType, Command> _actionBinding;

public:
	Player();

	void AssignKey(ActionType action, sf::Keyboard::Key key);

	void LoadSettings(const std::string& jsonFile);

	void HandleEvent(const sf::Event& ev, CommandQueue& commands);
	void HandleRealtimeInput(CommandQueue& commands);

private:
	bool IsRealtime(ActionType action) const;
};