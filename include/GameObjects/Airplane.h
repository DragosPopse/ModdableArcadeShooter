#pragma once

#include "GameObject.h"

#include <string>
#include <vector>

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

	float scale = 0;
	int hitpoints = 0;
	float speed = 0;
	sf::Texture* texture = nullptr;
	sf::Font* healthFont = nullptr;
	sf::IntRect idleRect;
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	std::vector<AiDirection> directions;
	std::vector<ProjectileData*> weapons;
	std::vector<int> ammo;
	int healthTextCharSize = 0;
	sol::optional<sf::Color> healthTextColor;

	sf::Vector2i explosionSize;
	int framesPerExplosion = 0;
	int numberOfExplosions = 0;
	sf::Texture* explosionsTexture = nullptr;
	float explosionFrameDuration = 0;
	float explosionMinScale = 0;
	float explosionMaxScale = 0;
	float explosionMaxRotation = 0;

	std::vector<RandomizedSound> explosionSounds;
	RandomizedSound switchSound;

	std::vector<DropData> drops;
	
	sol::optional<sol::function> onDestroy;

	int score = 0;
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

	float _weaponVisibilityElapsedTime;
	bool _fadingIn;

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

	void UpdateWeaponVisibility(float dt);
};