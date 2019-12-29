#include "GameObjects/Projectile.h"

#include <cmath>

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>

#include "Scene.h"
#include "Scenes/Level.h"

#include "GameObjects/ParticleSystemObject.h"
#include "GameObjects/Airplane.h"

#include "Utility/Math.h"
#include "Utility/Sol.h"
#include "Random.h"




Projectile::Projectile(ProjectileData* data) :
	_level(nullptr),
	_data(data),
	_playerControlled(false),
	_firstFrame(true),
	_rectChanged(false),
	_collided(false)
{
}


void Projectile::Start(Scene* scene)
{
	_level = static_cast<Level*>(scene);

	SetTexture(*_data->texture);
	SetTextureRect(_data->muzzleRect);
	setScale(_data->muzzleScale, _data->muzzleScale);

	_clock.restart();

	float randomAngle = RandReal(-_data->spreadAngle, _data->spreadAngle);
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

	_luaObject = Protect<sol::table>(_data->start(this));
	

	float defaultVolume = _level->GetContext()->player->GetSfxVolume();
	RandomizedSoundResult randSound = _data->muzzleSound(defaultVolume);

	_level->PlaySound(*randSound.buffer, randSound.volume, randSound.pitch);

	GameObject::Start(scene);
}


void Projectile::Update(float dt)
{
	if (!_rectChanged && _clock.getElapsedTime().asSeconds() > 0.02f)
	{
		_rectChanged = true;
		int randRect = RandInt(0, (int)_data->rects.size() - 1);
		//std::cout << randRect << '\n';
		SetTextureRect(_data->rects[randRect]);
		setScale(_data->scale, _data->scale);
	}

	if (_data->update.has_value())
	{
		Protect(_data->update.value().call(_luaObject, this, dt));
	}
	GameObject::Update(dt);
}


void Projectile::FixedUpdate(float dt)
{

	if (_data->fixedUpdate.has_value())
	{
		Protect(_data->fixedUpdate.value().call(_luaObject, this, dt));
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


int Projectile::GetDamage() const
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
	if (_data->onCollision.has_value())
	{
		_data->onCollision.value()(_luaObject, this, airplane);
	}
}


void Projectile::MarkForDestroy()
{
	if (_collided)
	{
		float defaultVolume = _level->GetContext()->player->GetSfxVolume();
		RandomizedSoundResult randSound = _data->destroySound(defaultVolume);

		_level->PlaySound(*randSound.buffer, randSound.volume, randSound.pitch);
	}

	GameObject::MarkForDestroy();
}