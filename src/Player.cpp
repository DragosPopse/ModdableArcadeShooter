#include "Player.h"

#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include "GameObject.h"
#include "GameObjects/Airplane.h"

#include "CommandQueue.h"
#include "Utility/String.h"


#define CONFIG_PATH "assets/config/"

namespace rjs = rapidjson;


namespace
{
	struct AircraftMoverX
	{
		int dirX;

		AircraftMoverX(int x) :
			dirX(x)
		{
		}


		void operator()(Airplane& airplane, float) const
		{
			airplane.MoveX(dirX);
		}
	};


	struct AircraftMoverY
	{
		int dirY;

		AircraftMoverY(int y) :
			dirY(y)
		{
		}


		void operator()(Airplane& airplane, float) const
		{
			airplane.MoveY(dirY);
		}
	};

	void AirplaneFire(Airplane& airplane, float)
	{
		airplane.Fire();
	}

	void AirplaneNextWeapon(Airplane& airplane, float)
	{
		airplane.NextWeapon();
	}

	void AirplanePreviousWeapon(Airplane& airplane, float)
	{
		airplane.PreviousWeapon();
	}
}


void Player::HandleEvent(const sf::Event& ev, CommandQueue& commands)
{
	if (_keyBinding.find(ev.key.code) != _keyBinding.end())
	{
		ActionType action = _keyBinding[ev.key.code];
		if (!IsRealtime(action))
		{
			commands.Push(_actionBinding[action]);
		}
	}
}


void Player::HandleRealtimeInput(CommandQueue& commands)
{
	for (const auto& pair : _keyBinding)
	{
		if (IsRealtime(pair.second) && sf::Keyboard::isKeyPressed(pair.first))
		{
			commands.Push(_actionBinding[pair.second]);
		}
	}
}


Player::Player() :
	_musicVolume(100),
	_sfxVolume(100)
{
	_keyBinding[sf::Keyboard::A] = ActionType::MoveLeft;
	_keyBinding[sf::Keyboard::D] = ActionType::MoveRight;
	_keyBinding[sf::Keyboard::W] = ActionType::MoveUp;
	_keyBinding[sf::Keyboard::S] = ActionType::MoveDown;
	_keyBinding[sf::Keyboard::Space] = ActionType::Fire;
	_keyBinding[sf::Keyboard::Q] = ActionType::PreviousWeapon;
	_keyBinding[sf::Keyboard::E] = ActionType::NextWeapon;
	_keyBinding[sf::Keyboard::Escape] = ActionType::Exit;

	Command moveLeft;
	moveLeft.category = GameObject::PlayerAirplane;
	moveLeft.action = DeriveAction<Airplane>(AircraftMoverX(-1));
	_actionBinding[ActionType::MoveLeft] = moveLeft;

	Command moveRight;
	moveRight.category = GameObject::PlayerAirplane;
	moveRight.action = DeriveAction<Airplane>(AircraftMoverX(1));
	_actionBinding[ActionType::MoveRight] = moveRight;

	Command moveUp;
	moveUp.category = GameObject::PlayerAirplane;
	moveUp.action = DeriveAction<Airplane>(AircraftMoverY(-1));
	_actionBinding[ActionType::MoveUp] = moveUp;

	Command moveDown;
	moveDown.category = GameObject::PlayerAirplane;
	moveDown.action = DeriveAction<Airplane>(AircraftMoverY(1));
	_actionBinding[ActionType::MoveDown] = moveDown;

	Command fire;
	fire.category = GameObject::PlayerAirplane;
	fire.action = DeriveAction<Airplane>(AirplaneFire);
	_actionBinding[ActionType::Fire] = fire;

	Command nextWeapon;
	nextWeapon.category = GameObject::PlayerAirplane;
	nextWeapon.action = DeriveAction<Airplane>(AirplaneNextWeapon);
	_actionBinding[ActionType::NextWeapon] = nextWeapon;

	Command previousWeapon;
	previousWeapon.category = GameObject::PlayerAirplane;
	previousWeapon.action = DeriveAction<Airplane>(AirplanePreviousWeapon);
	_actionBinding[ActionType::PreviousWeapon] = previousWeapon;
}


bool Player::IsRealtime(Player::ActionType action) const
{
	switch (action)
	{
	case ActionType::PreviousWeapon:
	case ActionType::NextWeapon:
		return false;
	default:
		return true;
	}
}


void Player::AssignKey(ActionType action, const sf::Keyboard::Key& key)
{
	//remove current binding for the action
	for (auto it = _keyBinding.begin(); it != _keyBinding.end(); )
	{
		if (it->second == action)
		{
			_keyBinding.erase(it++);
		}
		else
		{
			++it;
		}
	}

	_keyBinding[key] = action;
}


sf::Keyboard::Key Player::GetKey(ActionType action)
{
	auto found = std::find_if(_keyBinding.begin(), _keyBinding.end(), [action](auto& kv)
		{
			return kv.second == action;
		});
	return found->first;
}


void Player::LoadSettings()
{
	std::string fileName = BuildString(CONFIG_PATH, "Player.json");
	_keyBinding.clear();
	std::ifstream stream(fileName);
	rjs::IStreamWrapper wrapper(stream);

	rjs::Document document;
	document.ParseStream(wrapper);

	_keyBinding[(sf::Keyboard::Key)document["MoveLeft"].GetInt()] = ActionType::MoveLeft;
	_keyBinding[(sf::Keyboard::Key)document["MoveRight"].GetInt()] = ActionType::MoveRight;
	_keyBinding[(sf::Keyboard::Key)document["MoveUp"].GetInt()] = ActionType::MoveUp;
	_keyBinding[(sf::Keyboard::Key)document["MoveDown"].GetInt()] = ActionType::MoveDown;
	_keyBinding[(sf::Keyboard::Key)document["Fire"].GetInt()] = ActionType::Fire;
	_keyBinding[(sf::Keyboard::Key)document["NextWeapon"].GetInt()] = ActionType::NextWeapon;
	_keyBinding[(sf::Keyboard::Key)document["PreviousWeapon"].GetInt()] = ActionType::PreviousWeapon;
	_keyBinding[(sf::Keyboard::Key)document["Exit"].GetInt()] = ActionType::Exit;

	_sfxVolume = document["SfxVolume"].GetFloat();
	_musicVolume = document["MusicVolume"].GetFloat();
}


void Player::SaveSettings()
{
	std::string fileName = BuildString(CONFIG_PATH, "Player.json");
	
	rjs::Document document;
	document.SetObject();
	auto& allocator = document.GetAllocator();
	document.AddMember("SfxVolume", _sfxVolume, allocator);
	document.AddMember("MusicVolume", _musicVolume, allocator);
	for (const auto& pair : _keyBinding)
	{
		switch (pair.second)
		{
		case ActionType::MoveLeft:
			document.AddMember("MoveLeft", pair.first, allocator);
			break;

		case ActionType::MoveRight:
			document.AddMember("MoveRight", pair.first, allocator);
			break;

		case ActionType::MoveUp:
			document.AddMember("MoveUp", pair.first, allocator);
			break;

		case ActionType::MoveDown:
			document.AddMember("MoveDown", pair.first, allocator);
			break;

		case ActionType::Fire:
			document.AddMember("Fire", pair.first, allocator);
			break;

		case ActionType::NextWeapon:
			document.AddMember("NextWeapon", pair.first, allocator);
			break;

		case ActionType::PreviousWeapon:
			document.AddMember("PreviousWeapon", pair.first, allocator);
			break;

		case ActionType::Exit:
			document.AddMember("Exit", pair.first, allocator);
			break;
		}
	}

	std::ofstream stream(fileName);
	rjs::OStreamWrapper wrapper(stream);
	rjs::PrettyWriter writer(wrapper);
	document.Accept(writer);
}


bool Player::HasSettings() const
{
	std::ifstream file(BuildString(CONFIG_PATH, "Player.json"));
	return file.good();
}


std::string Player::Parse(const std::string& str)
{
	std::stringstream result;

	int i = 0;
	while (i < static_cast<int>(str.size()))
	{
		if (str[i] == '{')
		{
			std::stringstream param;
			int j = i + 1;
			if (j >= static_cast<int>(str.size()))
			{
				throw std::invalid_argument("Invalid Format");
			}

			while (str[j] != '}' && j < static_cast<int>(str.size()))
			{
				param << str[j];
				j++;
			}

			if (j == static_cast<int>(str.size()))
			{
				throw std::invalid_argument("Invalid Format");
			}

			i = j + 1;

			std::string sub = param.str();
			
			if (sub.compare("MoveLeft") == 0)
			{
				result << ToString(GetKey(ActionType::MoveLeft));
			}
			else if (sub.compare("MoveRight") == 0)
			{
				result << ToString(GetKey(ActionType::MoveRight));
			}
			else if (sub.compare("MoveUp") == 0)
			{
				result << ToString(GetKey(ActionType::MoveUp));
			}
			else if (sub.compare("MoveDown") == 0)
			{
				result << ToString(GetKey(ActionType::MoveDown));
			}
			else if (sub.compare("Fire") == 0)
			{
				result << ToString(GetKey(ActionType::Fire));
			}
			else if (sub.compare("NextWeapon") == 0)
			{
				result << ToString(GetKey(ActionType::NextWeapon));
			}
			else if (sub.compare("PreviousWeapon") == 0)
			{
				result << ToString(GetKey(ActionType::PreviousWeapon));
			}
			else if (sub.compare("Exit") == 0)
			{
				result << ToString(GetKey(ActionType::Exit));
			}
			else
			{
				throw std::invalid_argument(BuildString(sub, " is not a valid token"));
			}
		}
		else
		{
			result << str[i];
			i++;
		}
	}

	return result.str();
}


bool Player::Contains(const sf::Keyboard::Key& key)
{
	for (const auto& pair : _keyBinding)
	{
		if (pair.first == key)
		{
			return true;
		}
	}
	return false;
}