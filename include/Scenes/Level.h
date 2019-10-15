#pragma once

#include "Scene.h"
#include <string>
#include "Context.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"
#include <vector>
#include <map>
#include <deque>
#include "Player.h"
#include "CommandQueue.h"
#include "GameObjects/Animation.h"
#include "GameObjects/ParticleSystemObject.h"

class Airplane;
class Projectile;

struct AirplaneSpawnPosition
{
	AirplaneData* data;
	float x;
	float y;
};

class Level :
	public Scene
{
	std::vector<Airplane*> _airplanes;
	std::vector<Projectile*> _projectiles;

	std::unique_ptr<GameObject> _root;
	GameObject* _enemyProjectilesRoot;
	GameObject* _playerProjectilesRoot;
	GameObject* _enemyAirplanesRoot;
	GameObject* _explosionsRoot;
	GameObject* _environmentRoot;
	GameObject* _particlesRoot;
	std::unique_ptr<GameObject> _uiRoot;
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
	float _scale;

	float _fpsUpdateInterval;
	float _lastFpsUpdate;

	std::deque<AirplaneSpawnPosition> _enemySpawns;

public:
	Level(Context* context, const std::string& fileName);

	bool HandleEvent(const sf::Event& ev) override;
	bool FixedUpdate(float dt) override;
	bool Update(float dt) override;
	bool Render() override;

	void AddPlayerProjectile(Projectile* proj);
	void AddEnemyProjectile(Projectile* proj);
	void AddEnemyAirplane(Airplane* plane);
	void AddExplosion(Animation* expl);
	void AddParticles(ParticleSystemObject* p);
	void AddUiElement(GameObject* ui);

	GameObject* GetEnemyProjectilesRoot() { return _enemyProjectilesRoot; }
	GameObject* GetEnemyAirplanesRoot() { return _enemyAirplanesRoot; }
	Airplane* GetPlayerAirplane() { return _playerAirplane; }
	GameObject* GetPlayerProjectilesRoot() { return _playerProjectilesRoot; }
	

private:
	void SpawnEnemies();
	void RemoveOffScreenObjects(float dt);

	void HandleCollisions(float dt);
};