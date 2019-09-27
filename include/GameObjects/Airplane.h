#pragma once

#include "GameObject.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Level;

struct AiDirection
{
	float angle;
	float distance;
};

struct AirplaneData
{
	int hitpoints;
	std::string texture;
	sf::IntRect idleRect;
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	std::vector<AiDirection> directions;
};

class ProjectileData;

class Airplane :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;
	int _hitpoints;
	AirplaneData* _data;
	std::vector<ProjectileData*> _weapons;
	std::vector<int> _ammo;

public:
	Airplane(AirplaneData* data);

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Damage(int hp);
	void Repair(int hp);

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);
};