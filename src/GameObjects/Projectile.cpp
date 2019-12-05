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
#include "GameObjects/ParticleSystemObject.h"
#include <Thor/Math.hpp>


Projectile::Projectile(ProjectileData* data) :
	_level(nullptr),
	_data(data),
	_playerControlled(false),
	_firstFrame(true),
	_rectChanged(false),
	_collided(false)
{
	_luaObject = _data->create();
}


void Projectile::Start(Scene* scene)
{
	_level = static_cast<Level*>(scene);

	SetTexture(*_data->texture);
	SetTextureRect(_data->muzzleRect);
	setScale(_data->muzzleScale, _data->muzzleScale);

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

	_velocity = _direction * _data->speed;
	setRotation(randomAngle + 90);

	_data->start(_luaObject, this);

	float defaultVolume = _level->GetContext()->player->GetSfxVolume();
	RandomizedSoundResult randSound = _data->muzzleSound(_data->rng, defaultVolume);

	_level->PlaySound(*randSound.buffer, randSound.volume, randSound.pitch);

	GameObject::Start(scene);
}


void Projectile::Update(float dt)
{
	if (!_rectChanged && _clock.getElapsedTime().asSeconds() > 0.02f)
	{
		_rectChanged = true;
		int randRect = _data->rectDistribution(_data->rng);
		//std::cout << randRect << '\n';
		SetTextureRect(_data->rects[randRect]);
		setScale(_data->scale, _data->scale);
	}

	if (_data->update.has_value())
	{
		_data->update.value().call(_luaObject, this, dt);
	}
	GameObject::Update(dt);
}


void Projectile::FixedUpdate(float dt)
{

	if (_data->fixedUpdate.has_value())
	{
		_data->fixedUpdate.value().call(_luaObject, this, dt);
	}
	move(_velocity * dt);
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
	_collided = true;
	_data->onCollision(_luaObject, this, airplane);
}


void Projectile::MarkForDestroy()
{
	if (_collided)
	{
		float defaultVolume = _level->GetContext()->player->GetSfxVolume();
		RandomizedSoundResult randSound = _data->destroySound(_data->rng, defaultVolume);

		_level->PlaySound(*randSound.buffer, randSound.volume, randSound.pitch);
	}

	GameObject::MarkForDestroy();
}