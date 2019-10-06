#include "GameObjects/Projectile.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "GameObjects/Airplane.h"
#include <sol/sol.hpp>
#include <random>
#include "Utility.h"
#include <cmath>


Projectile::Projectile(ProjectileData* data) :
	_currentScene(nullptr),
	_data(data),
	_playerControlled(false),
	_firstFrame(true),
	_rectChanged(false)
{
}


void Projectile::Start(Scene* scene)
{
	_currentScene = static_cast<Level*>(scene);
	SetTexture(_currentScene->GetTextures()[_data->texture]);
	SetTextureRect(_data->muzzleRect);
	setScale(2, 2);
	_clock.restart();

	float randomAngle = _data->generator(_data->rng);
	if (_playerControlled)
	{
		randomAngle -= 90;
	}
	else
	{
		randomAngle += 90;
	}
	
	float speed = _data->speed;
	float radian = ToRadian(randomAngle);
	float x = speed * cos(radian);
	float y = speed * sin(radian);
	_direction.x = x;
	_direction.y = y;
	_direction = Normalize(_direction);

	setRotation(randomAngle + 90);
	std::cout << "ANGLE: " << randomAngle << '\n';
	GameObject::Start(scene);
}


void Projectile::Update(float dt)
{
	/*if (_firstFrame)
	{
		_rectChanged = false;
		_firstFrame = false;
	}
	else if (!_rectChanged)
	{
		_rectChanged = true;
		SetTextureRect(_data->rect);
		std::cout << "C\n";
	}*/

	if (!_rectChanged && _clock.getElapsedTime().asSeconds() > 0.02f)
	{
		_rectChanged = true;
		SetTextureRect(_data->rect);
		setScale(1, 1);
	}

	if (_data->update.has_value())
	{
		Projectile::Context pContext;
		pContext.deltaTime = dt;
		_data->update.value().call(this, pContext);
	}
	GameObject::Update(dt);
}


void Projectile::FixedUpdate(float dt)
{

	if (_data->fixedUpdate.has_value())
	{
		Projectile::Context pContext;
		pContext.deltaTime = dt;
		_data->fixedUpdate.value().call(this, pContext);
	}
	else
	{
		move(_direction * _data->speed * dt);
	}
	GameObject::FixedUpdate(dt);
}


void Projectile::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}


void Projectile::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
	CenterOrigin(_sprite);
}


void Projectile::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
	CenterOrigin(_sprite);
}


float Projectile::GetSpeed() const
{
	return _data->speed;
}


float Projectile::GetDamage() const
{
	return _data->damage;
}


sf::Vector2f Projectile::GetVelocity() const
{
	return _velocity;
}


void Projectile::SetVelocity(const sf::Vector2f& velocity)
{
	_velocity = velocity;
}


void Projectile::SetRotation(float rotation)
{
	_sprite.setRotation(rotation);
}


unsigned int Projectile::GetCategory() const
{
	if (_playerControlled)
	{
		return Type::PlayerProjectile;
	}
	return Type::EnemyProjectile;
}


sf::FloatRect Projectile::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(_sprite.getGlobalBounds());
}


void Projectile::OnCollision(Airplane* airplane)
{
	_data->onCollision(this, airplane);
}