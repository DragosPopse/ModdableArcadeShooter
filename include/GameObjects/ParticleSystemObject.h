#pragma once


#include "GameObject.h"
#include <Thor/Particles.hpp>


struct ParticleSystemData
{

};


class ParticleSystemObject :
	public GameObject
{
	float _lifetime;
	float _elapsedTime;
	bool _removeAfterLifetime;

public:
	thor::ParticleSystem system;

	ParticleSystemObject();

	void Start(Scene* scene) override;

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void SetRemoveAfterLifetime(float time);
};