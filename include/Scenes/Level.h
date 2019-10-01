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

	std::map<std::string, AirplaneData> _airplaneDataDict;
	std::map<std::string, ProjectileData> _projectileDataDict;

	Player _player;
	CommandQueue _commands;

public:
	Level(Context* context, const std::string& fileName);

	bool HandleEvent(const sf::Event& ev) override;
	bool FixedUpdate(float dt) override;
	bool Update(float dt) override;
	bool Render() override;

	void SpawnEnemies();
	void RemoveOffScreenObjects();
};