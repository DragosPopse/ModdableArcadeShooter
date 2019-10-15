#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include <optional>
#include <random>

class Level;
class Airplane;

struct ProjectileData
{
	float scale;
	float muzzleScale;
	float iconScale;

	std::string texture;
	std::string iconTexture;
	sf::IntRect iconRect;
	sf::IntRect rect;
	sf::IntRect muzzleRect;
	float speed;
	int damage;
	float fireRate;
	float spreadAngle;
	sol::optional<sol::function> fixedUpdate;
	sol::optional<sol::function> update;
	sol::function onCollision;
	sol::optional<sol::function> onDestroy;

	std::mt19937 rng;
	std::uniform_real_distribution<float> generator;
};


class Projectile :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;
	ProjectileData* _data;
	sf::Vector2f _velocity;
	bool _playerControlled;

	bool _firstFrame;
	bool _rectChanged;

	sf::Clock _clock;
	

	sf::Vector2f _direction;

public:

	Projectile(ProjectileData* data);

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);

	float GetSpeed() const;
	float GetDamage() const;

	Level* GetLevel() { return _currentScene; }

	sf::Vector2f GetVelocity() const;
	void SetVelocity(const sf::Vector2f& velocity);

	bool IsPlayerControlled() const { return _playerControlled; }
	void SetPlayerControlled(bool b) { _playerControlled = b; }

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;

	void OnCollision(Airplane* airplane);
};