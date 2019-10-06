#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include <optional>

class Level;
class Airplane;


struct ProjectileData
{
	std::string texture;
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
};


class Projectile :
	public GameObject
{
	Level* _currentScene;
	sf::Sprite _sprite;
	ProjectileData* _data;
	sf::Vector2f _velocity;
	bool _playerControlled;
	int _direction;

	bool _firstFrame;
	bool _rectChanged;

	sf::Clock _clock;

public:
	struct Context
	{
		float deltaTime;
	};

	Projectile(ProjectileData* data);

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);

	float GetSpeed() const;
	float GetDamage() const;

	sf::Vector2f GetVelocity() const;
	void SetVelocity(const sf::Vector2f& velocity);
	void SetRotation(float rotation);

	bool IsPlayerControlled() const { return _playerControlled; }
	void SetPlayerControlled(bool b) { _playerControlled = b; }
	int GetDirection() const { return _direction; }

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;

	void OnCollision(Airplane* airplane);
};