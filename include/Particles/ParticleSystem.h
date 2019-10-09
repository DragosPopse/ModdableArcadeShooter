#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>
#include "Particle.h"


struct ParticleSystemData
{
	std::string texture;
};


class ParticleSystem :
	public GameObject
{
	mutable sf::VertexArray _vertices;
	std::deque<Particle> _particles;
	mutable bool _updateVertices;

public:
	ParticleSystem();

	void Start(Scene* scene) override;

	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void UpdateVertices() const;
};