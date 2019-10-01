#include "Player.h"
#include <SFML/Graphics.hpp>
#include "CommandQueue.h"
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include <functional>


struct AircraftMover
{
	float dirX;
	float dirY;

	AircraftMover(float x, float y) :
		dirX(x),
		dirY(y)
	{
	}


	void operator()(Airplane& airplane, float dt)
	{
		airplane.MoveInDirection(dirX, dirY, dt);
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
	moveLeft.action = DeriveAction<Airplane>(AircraftMover(-1, 0));
	_actionBinding[MoveLeft] = moveLeft;

	Command moveRight;
	moveRight.category = GameObject::PlayerAirplane;
	moveRight.action = DeriveAction<Airplane>(AircraftMover(1, 0));
	_actionBinding[MoveLeft] = moveRight;

	Command moveUp;
	moveUp.category = GameObject::PlayerAirplane;
	moveUp.action = DeriveAction<Airplane>(AircraftMover(0, -1));
	_actionBinding[MoveLeft] = moveUp;

	Command moveDown;
	moveDown.category = GameObject::PlayerAirplane;
	moveDown.action = DeriveAction<Airplane>(AircraftMover(0, 1));
	_actionBinding[MoveLeft] = moveDown;

	Command fire;
	fire.category = GameObject::PlayerAirplane;
	fire.action = DeriveAction<Airplane>(AirplaneFire);
	_actionBinding[Fire] = fire;

	Command nextWeapon;
	nextWeapon.category = GameObject::PlayerAirplane;
	nextWeapon.action = DeriveAction<Airplane>(AirplaneNextWeapon);
	_actionBinding[Fire] = nextWeapon;

	Command previousWeapon;
	previousWeapon.category = GameObject::PlayerAirplane;
	previousWeapon.action = DeriveAction<Airplane>(AirplanePreviousWeapon);
	_actionBinding[Fire] = previousWeapon;
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