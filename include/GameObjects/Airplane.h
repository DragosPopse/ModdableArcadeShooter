#pragma once

#include "GameObject.h"

class Level;


class Airplane :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;
	int _hitpoints;

public:
	Airplane();

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Damage(int hp);
	void Repair(int hp);

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);
};