#pragma once

#include "GameObject.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObjects/SpriteObject.h"
#include <random>
#include <Thor/Animations.hpp>

class Level;
class ProjectileData;

struct AiDirection
{
	float angle;
	float distance;
};

struct AirplaneData
{
	float scale;
	int hitpoints;
	float speed;
	std::string texture;
	std::string healthFont;
	sf::IntRect idleRect;
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	std::vector<AiDirection> directions;
	std::vector<ProjectileData*> weapons;
	std::vector<int> ammo;
	int healthTextCharSize;

	sf::Vector2i explosionSize;
	int framesPerExplosion;
	int numberOfExplosions;
	std::string explosionsTexture;
	float explosionFrameDuration;
	float explosionMinScale;
	float explosionMaxScale;

	std::mt19937 rng;
	std::uniform_real_distribution<float> generator;
};

class TextObject;

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

	TextObject* _healthText;
	SpriteObject* _weaponDisplay;

	float _elapsedTime;
	bool _startDmgAnimation;
	bool _dmgAnimationActive;
	const sf::Texture* _texture;
	sf::Shader* _shader;

public:
	Airplane(AirplaneData* data);

	void Start(Scene* scene) override;

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
	void SetShader(sf::Shader* shader);

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;

private:
	void UpdateHealthDisplay();
	void UpdateWeaponDisplay();
};