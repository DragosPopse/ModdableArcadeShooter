#pragma once

#include "Scene.h"
#include <string>
#include "Context.h"
#include <vector>

class Airplane;
class Projectile;

class Level :
	public Scene
{
	std::vector<Airplane*> _airplanes;
	std::vector<Projectile*> _projectiles;

	std::unique_ptr<GameObject> _root;

public:
	Level(Context* context, const std::string& fileName);

	bool FixedUpdate(float dt) override;
	bool Update(float dt) override;
	bool Render() override;

	void SpawnEnemies();
	void RemoveOffScreenObjects();
};