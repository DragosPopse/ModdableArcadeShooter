#include "Particles/ParticleSystem.h"


ParticleSystem::ParticleSystem() :
	_vertices(sf::Quads),
	_updateVertices(false)
{
}


void ParticleSystem::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (_updateVertices)
	{
		UpdateVertices();
		_updateVertices = false;
	}
	
	target.draw(_vertices, states);

	GameObject::Draw(target, states);
}


void ParticleSystem::Start(Scene* scene)
{
	
}


void ParticleSystem::FixedUpdate(float dt)
{
	
}


void ParticleSystem::UpdateVertices() const
{
	
}