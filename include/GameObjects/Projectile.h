#pragma once

#include "GameObject.h"

class Level;


class Projectile :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;

public:
	Projectile();

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};