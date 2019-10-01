#include "Player.h"
#include <SFML/Graphics.hpp>
#include "CommandQueue.h"
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include <functional>
#include <iostream>

#include "Utility.h"

#include <fstream>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace rjs = rapidjson;


namespace
{
	struct AircraftMoverX
	{
		int dirX;

		AircraftMoverX(float x) :
			dirX(x)
		{
		}


		void operator()(Airplane& airplane, float dt) const
		{
			airplane.MoveX(dirX);
		}
	};


	struct AircraftMoverY
	{
		int dirY;

		AircraftMoverY(float y) :
			dirY(y)
		{
		}


		void operator()(Airplane& airplane, float dt) const
		{
			airplane.MoveY(dirY);
		}
	};

	void AirplaneFire(Airplane& airplane, float dt)
	{
		airplane.Fire();
	}

	void AirplaneNextWeapon(Airplane& airplane, float dt)
	{
		airplane.NextWeapon();
	}

	void AirplanePreviousWeapon(Airplane& airplane, float dt)
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
	for (auto it = _keyBinding.begin(); it != _keyBinding.end(); it++)
	{
		if (IsRealtime(it->second) && sf::Keyboard::isKeyPressed(it->first))
		{
			commands.Push(_actionBinding[it->second]);
		}
	}
}


Player::Player()
{
	_keyBinding[sf::Keyboard::A] = MoveLeft;
	_keyBinding[sf::Keyboard::D] = MoveRight;
	_keyBinding[sf::Keyboard::W] = MoveUp;
	_keyBinding[sf::Keyboard::S] = MoveDown;
	_keyBinding[sf::Keyboard::Space] = Fire;
	_keyBinding[sf::Keyboard::Q] = PreviousWeapon;
	_keyBinding[sf::Keyboard::E] = NextWeapon;

	Command moveLeft;
	moveLeft.category = GameObject::PlayerAirplane;
	moveLeft.action = DeriveAction<Airplane>(AircraftMoverX(-1));
	_actionBinding[MoveLeft] = moveLeft;

	Command moveRight;
	moveRight.category = GameObject::PlayerAirplane;
	moveRight.action = DeriveAction<Airplane>(AircraftMoverX(1));
	_actionBinding[MoveRight] = moveRight;

	Command moveUp;
	moveUp.category = GameObject::PlayerAirplane;
	moveUp.action = DeriveAction<Airplane>(AircraftMoverY(-1));
	_actionBinding[MoveUp] = moveUp;

	Command moveDown;
	moveDown.category = GameObject::PlayerAirplane;
	moveDown.action = DeriveAction<Airplane>(AircraftMoverY(1));
	_actionBinding[MoveDown] = moveDown;

	Command fire;
	fire.category = GameObject::PlayerAirplane;
	fire.action = DeriveAction<Airplane>(AirplaneFire);
	_actionBinding[Fire] = fire;

	Command nextWeapon;
	nextWeapon.category = GameObject::PlayerAirplane;
	nextWeapon.action = DeriveAction<Airplane>(AirplaneNextWeapon);
	_actionBinding[NextWeapon] = nextWeapon;

	Command previousWeapon;
	previousWeapon.category = GameObject::PlayerAirplane;
	previousWeapon.action = DeriveAction<Airplane>(AirplanePreviousWeapon);
	_actionBinding[PreviousWeapon] = previousWeapon;
}


bool Player::IsRealtime(Player::ActionType action) const
{
	switch (action)
	{
	case PreviousWeapon:
	case NextWeapon:
		return false;
	default:
		return true;
	}
}


void Player::AssignKey(ActionType action, sf::Keyboard::Key key)
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


void Player::LoadSettings()
{
	std::string fileName = BuildString(CONFIG_PATH, "KeyBindings.json");
	std::ifstream stream(fileName);
	rjs::IStreamWrapper wrapper(stream);

	rjs::Document document;
	document.ParseStream(wrapper);

	_keyBinding[(sf::Keyboard::Key)document["MoveLeft"].GetInt()] = MoveLeft;
	_keyBinding[(sf::Keyboard::Key)document["MoveRight"].GetInt()] = MoveRight;
	_keyBinding[(sf::Keyboard::Key)document["MoveUp"].GetInt()] = MoveUp;
	_keyBinding[(sf::Keyboard::Key)document["MoveDown"].GetInt()] = MoveDown;
	_keyBinding[(sf::Keyboard::Key)document["Fire"].GetInt()] = Fire;
	_keyBinding[(sf::Keyboard::Key)document["NextWeapon"].GetInt()] = NextWeapon;
	_keyBinding[(sf::Keyboard::Key)document["PreviousWeapon"].GetInt()] = PreviousWeapon;
}


void Player::SaveSettings()
{
	std::string fileName = BuildString(CONFIG_PATH, "KeyBindings.json");
	
	rjs::Document document;
	document.SetObject();
	auto& allocator = document.GetAllocator();
	
	for (auto it = _keyBinding.begin(); it != _keyBinding.end(); ++it)
	{
		switch (it->second)
		{
		case MoveLeft:
			
			document.AddMember("MoveLeft", it->first, allocator);
			break;

		case MoveRight:		
			document.AddMember("MoveRight", it->first, allocator);
			break;

		case MoveUp:	
			document.AddMember("MoveUp", it->first, allocator);
			break;

		case MoveDown:			
			document.AddMember("MoveDown", it->first, allocator);
			break;

		case Fire:
			document.AddMember("Fire", it->first, allocator);
			break;

		case NextWeapon:
			document.AddMember("NextWeapon", it->first, allocator);
			break;

		case PreviousWeapon:
			document.AddMember("PreviousWeapon", it->first, allocator);
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
	std::ifstream file(BuildString(CONFIG_PATH, "KeyBindings.json"));
	return file.good();
}