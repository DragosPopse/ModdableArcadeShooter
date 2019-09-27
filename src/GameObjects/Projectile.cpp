#include "GameObjects/Projectile.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>


Projectile::Projectile() :
	_currentScene(nullptr)
{
}


void Projectile::Start(Scene* scene)
{
	_currentScene = static_cast<Level*>(scene);

	GameObject::Start(scene);
}


void Projectile::Update(float dt)
{
	GameObject::Update(dt);
}


void Projectile::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}


void Projectile::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}