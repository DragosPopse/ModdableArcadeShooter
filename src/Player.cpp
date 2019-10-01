#include "Player.h"
#include <SFML/Graphics.hpp>
#include "CommandQueue.h"
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include <functional>
#include <iostream>


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


void Player::LoadSettings(const std::string& jsonFile)
{

}


void Player::SaveSettings(const std::string& jsonFile)
{

}