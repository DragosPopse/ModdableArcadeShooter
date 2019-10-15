#include "GameObjects/ParticleSystemObject.h"
#include <Thor/Math.hpp>


ParticleSystemObject::ParticleSystemObject() :
	_elapsedTime(0),
	_removeAfterLifetime(false)
{
}


void ParticleSystemObject::Start(Scene* scene)
{
	GameObject::Start(scene);
}


void ParticleSystemObject::Update(float dt)
{
	system.update(sf::seconds(dt));
	if (_removeAfterLifetime)
	{
		_elapsedTime += dt;
		if (_elapsedTime > _lifetime)
		{
			MarkForDestroy();
		}
	}
	GameObject::Update(dt);
}


void ParticleSystemObject::FixedUpdate(float dt)
{
	//GameObject::FixedUpdate(dt);
}


void ParticleSystemObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(system, states);

	GameObject::Draw(target, states);

}


void ParticleSystemObject::SetRemoveAfterLifetime(float time)
{
	_removeAfterLifetime = true;
	_lifetime = time;
}


ParticleSystemObject::~ParticleSystemObject()
{
	std::cout << "destroyed\n";
}