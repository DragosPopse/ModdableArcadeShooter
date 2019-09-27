#include "GameObjects/Projectile.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>


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
	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}


void Projectile::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
}


void Projectile::SetTextureRect(const sf::IntRect& rect)
{
	_sprite.setTextureRect(rect);
}


float Projectile::GetSpeed() const
{
	return _data->speed;
}


float Projectile::GetDamage() const
{
	return _data->damage;
}