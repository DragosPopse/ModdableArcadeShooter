#include "GameObjects/Airplane.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>
#include "Utility.h"


Airplane::Airplane(AirplaneData* data) :
	_currentScene(nullptr),
	_data(data),
	_hitpoints(data->hitpoints),
	_currentWeaponIndex(0),
	_playerControlled(false),
	_cooldown(0.f),
	_moveX(0),
	_moveY(0),
	_moved(false)
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
	
	SetTexture(textures[_data->texture]);
	SetTextureRect(_data->idleRect);
	
	GameObject::Start(scene);
}


void Airplane::Update(float dt)
{
	_cooldown += dt;
	GameObject::Update(dt);
}


void Airplane::FixedUpdate(float dt)
{
	if (_moveX == 0)
	{
		SetTextureRect(_data->idleRect);
	}
	else if (_moveX > 0)
	{
		SetTextureRect(_data->rightRect);
	}
	else
	{
		SetTextureRect(_data->leftRect);
	}

	if (_moved)
	{
		move(Normalize(sf::Vector2f(_moveX, _moveY)) * _data->speed * dt);
	}
	_moveX = 0;
	_moveY = 0;
	_moved = false;
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
	CenterOrigin(_sprite);
}


void Airplane::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
	CenterOrigin(_sprite);
}


void Airplane::SetPlayerControlled(bool b)
{
	_playerControlled = b;
}


unsigned int Airplane::GetCategory() const
{
	if (_playerControlled)
	{
		return Type::PlayerAirplane;
	}
	return Type::EnemyAirplane;
}


void Airplane::NextWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex++;
	if (_currentWeaponIndex >= _ammo.size())
	{
		_currentWeaponIndex = 0;
	}
}


void Airplane::PreviousWeapon()
{
	_cooldown = 0;
	_currentWeaponIndex--;
	if (_currentWeaponIndex < 0)
	{
		_currentWeaponIndex = _ammo.size() - 1;
	}
}


void Airplane::Fire()
{
	if (_ammo[_currentWeaponIndex] > 0 && _cooldown > _data->weapons[_currentWeaponIndex]->fireRate)
	{
		_cooldown = 0;
		_ammo[_currentWeaponIndex]--;

		ProjectileData* projectileData = _data->weapons[_currentWeaponIndex];
		Projectile* proj = new Projectile(projectileData);
		proj->setPosition(GetWorldPosition());

		if (_playerControlled)
		{
			proj->SetPlayerControlled(true);
			_currentScene->AddPlayerProjectile(proj);
			proj->move(0, -GetBoundingRect().height / 2);
		}
		else
		{
			proj->SetRotation(180);
			_currentScene->AddEnemyProjectile(proj);
			proj->move(0, GetBoundingRect().height / 2);
		}

	}
}


void Airplane::MoveX(int x)
{
	_moveX = x;
	_moved = true;
}


void Airplane::MoveY(int y)
{
	_moveY = y;
	_moved = true;
}


sf::FloatRect Airplane::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(_sprite.getGlobalBounds());
}