#pragma once

#include "GameObject.h"

#include <optional>

#include <SFML/Graphics.hpp>
#include  <SFML/Audio.hpp>
#include <sol/sol.hpp>

#include "RandomizedSound.h"

class Level;
class Airplane;

struct ProjectileData
{
	std::string name;

	float scale;
	float muzzleScale;
	float iconScale;

	sf::Texture* texture;
	sf::Texture* iconTexture;
	sf::IntRect iconRect;
	std::vector<sf::IntRect> rects;
	sf::IntRect muzzleRect;
	float speed;
	int damage;
	float fireRate;
	float spreadAngle;
	sol::optional<sol::function> fixedUpdate;
	sol::optional<sol::function> update;
	sol::optional<sol::function> onCollision;
	sol::optional<sol::function> onDestroy; //WIP
	sol::function start;


	int ammoTextSize;
	sf::Font* ammoFont;

	RandomizedSound muzzleSound;
	RandomizedSound destroySound;
};


class Projectile :
	public GameObject
{
	Level* _level;
	sf::Sprite _sprite;
	ProjectileData* _data;
	sf::Vector2f _velocity;
	bool _playerControlled;

	bool _firstFrame;
	bool _rectChanged;


	sf::Clock _clock;
	

	sf::Vector2f _direction;

	sol::table _luaObject;

	bool _collided;

public:

	Projectile(ProjectileData* data);

	void Start(Scene* scene);

	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);

	float GetSpeed() const;
	int GetDamage() const;

	Level* GetLevel() { return _level; }

	sf::Vector2f GetVelocity() const;
	void SetVelocity(const sf::Vector2f& velocity);

	bool IsPlayerControlled() const { return _playerControlled; }
	void SetPlayerControlled(bool b) { _playerControlled = b; }

	sf::FloatRect GetBoundingRect() const override;

	unsigned int GetCategory() const override;

	void OnCollision(Airplane* airplane);

	void MarkForDestroy() override;
};