#include "GameObjects/Airplane.h"
#include "Scene.h"
#include "Scenes/Level.h"
#include <SFML/Graphics.hpp>


Airplane::Airplane(AirplaneData* data) :
	_currentScene(nullptr),
	_data(data),
	_hitpoints(_data->hitpoints)
{
	
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

	GameObject::Update(dt);
}


void Airplane::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}


void Airplane::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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