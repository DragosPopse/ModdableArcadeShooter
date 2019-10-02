#include "GameObjects/Projectile.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "GameObjects/Airplane.h"


Projectile::Projectile(ProjectileData* data) :
	_currentScene(nullptr),
	_data(data)
{
}


void Projectile::Start(Scene* scene)
{
	_currentScene = static_cast<Level*>(scene);

	GameObject::Start(scene);
}


void Projectile::Update(float dt)
{
	if (_data->update)
	{
		sol::table table;
		table["deltaTime"] = dt;
		_data->update.value().call(this, table);
	}
	GameObject::Update(dt);
}


void Projectile::FixedUpdate(float dt)
{
	if (_data->fixedUpdate)
	{
		sol::table table;
		table["deltaTime"] = dt;
		_data->fixedUpdate.value().call(this, table);
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