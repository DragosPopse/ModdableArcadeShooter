#pragma once

#include "GameObject.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Level;
class ProjectileData;

struct AiDirection
{
	float angle;
	float distance;
};

struct AirplaneData
{
	int hitpoints;
	float speed;
	std::string texture;
	sf::IntRect idleRect;
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	std::vector<AiDirection> directions;
	std::vector<ProjectileData*> weapons;
	std::vector<int> ammo;
};


class Airplane :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;
	int _hitpoints;
	AirplaneData* _data;
	std::vector<int> _ammo;
	int _currentWeaponIndex;
	bool _playerControlled;
	float _cooldown;
	
	int _moveX;
	int _moveY;
	bool _moved;

	float _currentPatternAngle;
	float _currentPatternDistance;
	float _distanceMoved;
	int _currentPatternIndex;

public:
	Airplane(AirplaneData* data);

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Damage(int hp);
	void Repair(int hp);

	void MoveX(int x);
	void MoveY(int y);

	void Fire();
	void NextWeapon();
	void PreviousWeapon();

	int GetHealth() const { return _hitpoints; }

	void SetPlayerControlled(bool b);

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;
};