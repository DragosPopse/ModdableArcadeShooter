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

	float _sfxVolume;
	float _musicVolume;

public:
	Player();

	void AssignKey(ActionType action, sf::Keyboard::Key key);
	sf::Keyboard::Key GetKey(ActionType action);

	void LoadSettings();
	void SaveSettings();
	bool HasSettings() const;

	void HandleEvent(const sf::Event& ev, CommandQueue& commands);
	void HandleRealtimeInput(CommandQueue& commands);

	float GetSfxVolume() const { return _sfxVolume; }
	float GetMusicVolume() const { return _musicVolume; }
	void SetSfxVolume(float v) { _sfxVolume = v; }
	void SetMusicVolume(float v) { _musicVolume = v; }

	/*
		Available tokens (words inside {} brackets): 
			MoveLeft
			MoveRight
			MoveUp
			MoveDown
			Fire
			NextWeapon
			PreviousWeapon
	*/
	std::string Parse(const std::string& str);

private:
	bool IsRealtime(ActionType action) const;
};