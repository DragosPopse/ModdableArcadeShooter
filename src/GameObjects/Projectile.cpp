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
	ParticleSystemObject* pso = new ParticleSystemObject();
	auto& ps = pso->system;
	pso->SetRemoveAfterLifetime(3);
	ps.setTexture(_currentScene->GetTextures()["Fragments"]);
	for (int i = 0; i < 4; i++)
	{
		sf::IntRect rect;
		rect.width = 5;
		rect.height = 5;
		rect.top = 0;
		rect.left = i * 5;
		ps.addTextureRect(rect);
	}
	thor::UniversalEmitter em;
	em.setEmissionRate(20);
	//std::cout << _direction.x << ' ' << _direction.y << '\n';
	//em.setParticleVelocity(thor::Distributions::deflect(_direction * 120.f, 10));
	em.setParticleVelocity(thor::Distributions::circle(sf::Vector2f(0, 0), 20));
	em.setParticleLifetime(sf::seconds(0.3));
	em.setParticleRotation(thor::Distributions::uniform(0, 360));
	em.setParticleTextureIndex(thor::Distributions::uniform(0, 3));
	em.setParticleScale(UniformVector2fDistribution(1, 2));
	ps.addEmitter(em, sf::seconds(0.2));
	pso->setPosition(GetWorldPosition());

	_currentScene->AddParticles(pso);
}