#include "GameObjects/Airplane.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>


Airplane::Airplane(AirplaneData* data) :
	_currentScene(nullptr),
	_data(data),
	_hitpoints(data->hitpoints),
	_currentWeaponIndex(0),
	_playerControlled(false)
{
	for (int i = 0; i < _data->ammo.size(); i++)
	{
		_ammo.push_back(_data->ammo[i]);
	}
}


void Airplane::Start(Scene* scene)
{
	_currentScene = static_cast<Level*>(scene);
	auto& textures = _currentScene->GetTextures();
	
	_sprite.setTexture(textures[_data->texture]);
	_sprite.setTextureRect(_data->idleRect);
	
	GameObject::Start(scene);
}


void Airplane::Update(float dt)
{
	if (_playerControlled)
	{
		_leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		_rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	}
	GameObject::Update(dt);
}


void Airplane::FixedUpdate(float dt)
{
	if (_playerControlled)
	{
		int direction = 0;
		if (_leftPressed)
		{
			direction = -1;
		}
		else if (_rightPressed)
		{
			direction = 1;
		}
		std::cout << _leftPressed << ' ' << _rightPressed << ' ' << dt << '\n';

		move(direction * _data->speed * dt, 0);
	}
	GameObject::FixedUpdate(dt);
}


void Airplane::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}


void Airplane::Damage(int hp)
{
	_hitpoints -= hp;
	if (_hitpoints <= 0)
	{
		MarkForDestroy();
	}
}


void Airplane::Repair(int hp)
{
	_hitpoints += hp;
}


void Airplane::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
}


void Airplane::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
}


void Airplane::SetPlayerControlled(bool b)
{
	_playerControlled = b;
}