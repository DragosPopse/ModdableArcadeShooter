#pragma once

#include "GameObject.h"

#include <string>
#include <vector>
#include <random>

#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>

#include "GameObjects/SpriteObject.h"
#include "GameObjects/CircleCooldown.h"
#include "GameObjects/TextObject.h"

#include "RandomizedSound.h"

class Level;
struct ProjectileData;
class Pickup;
struct PickupData;

struct AiDirection
{
	float angle;
	float distance;
};

struct DropData
{
	int dropRate;
	PickupData* pickup;
};


struct AirplaneData
{
	std::string name;

	float scale;
	int hitpoints;
	float speed;
	sf::Texture* texture;
	sf::Font* healthFont;
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
	sf::Texture* explosionsTexture;
	float explosionFrameDuration;
	float explosionMinScale;
	float explosionMaxScale;
	float explosionMaxRotation;

	std::vector<RandomizedSound> explosionSounds;
	RandomizedSound switchSound;

	std::vector<DropData> drops;
	
	std::mt19937 rng;
	std::uniform_real_distribution<float> scaleDistribution;
	std::uniform_int_distribution<int> dropDistribution;
	std::uniform_int_distribution<int> explosionSpriteDistribution;
	std::uniform_int_distribution<int> explosionSoundDistribution;

	sol::optional<sol::function> onDestroy;

	int score;
};


class TextObject;

class Airplane :
	public GameObject
{
	Level* _level;
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
	TextObject* _ammoDisplay;
	CircleCooldown* _cooldownDisplay;

	float _elapsedTime;
	bool _startDmgAnimation;
	bool _dmgAnimationActive;
	const sf::Texture* _texture;
	sf::Shader* _shader;

	bool _blockShooting = false;

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

	void SetBlockShooting(bool block);

	void Fire();
	void NextWeapon();
	void PreviousWeapon();

	Level* GetLevel() { return _level; }

	int GetHealth() const { return _hitpoints; }
	void AddHealth(int n);

	void SetPlayerControlled(bool b);

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);
	void SetShader(sf::Shader* shader);

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;

	void AddAmmo(const std::string& projectile, int n);


private:
	void UpdateHealthDisplay();
	void UpdateWeaponDisplay();
	void UpdateCooldownDisplay();
	void UpdateCooldownVertices();
	void UpdateAmmoDisplay();
};