#pragma once

#include "Scene.h"
#include <string>
#include "Context.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"
#include <vector>
#include <map>
#include "Player.h"
#include "CommandQueue.h"

class Airplane;
class Projectile;

class Level :
	public Scene
{
	std::vector<Airplane*> _airplanes;
	std::vector<Projectile*> _projectiles;

	std::unique_ptr<GameObject> _root;
	GameObject* _enemyProjectilesRoot;
	GameObject* _playerProjectilesRoot;
	GameObject* _enemyAirplanesRoot;
	Airplane* _playerAirplane;

	std::map<std::string, AirplaneData> _airplaneDataDict;
	std::map<std::string, ProjectileData> _projectileDataDict;

	Player _player;
	CommandQueue _commands;

	sf::View _worldView;
	sf::FloatRect _worldBounds;
	sf::Vector2f _playerSpawn;
	float _scrollSpeed;
	float _worldHeight;

	float _fpsUpdateInterval;
	float _lastFpsUpdate;

public:
	Level(Context* context, const std::string& fileName);

	bool HandleEvent(const sf::Event& ev) override;
	bool FixedUpdate(float dt) override;
	bool Update(float dt) override;
	bool Render() override;

private:
	void SpawnEnemies();
	void RemoveOffScreenObjects();

	void HandleCollisions(float dt);
};