#include "Scenes/Level.h"
#include <string>
#include <iostream>
#include "Context.h"
#include <sol/sol.hpp>
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"
#include <sol/sol.hpp>
#include "Utility.h"
#include <vector>
#include <memory>

namespace
{
	sf::IntRect TableToRect(const sol::table& table)
	{
		sf::IntRect result;
		result.left = table[1];
		result.top = table[2];
		result.width = table[3];
		result.height = table[4];
		return result;
	}
}

Level::Level(Context* context, const std::string& fileName) :
	Scene(context),
	_root(new GameObject())
{
	sol::table level = _context->lua->do_file(BuildString(LEVELS_PATH, fileName));

	//Load needed textures
	sol::table usedTextures = level["usedTextures"];
	for (int i = 1; i <= usedTextures.size(); i++)
	{
		sol::table texture = usedTextures[i];
		std::string id = texture[1];

		std::string file = texture[2];
		std::string path = BuildString(TEXTURES_PATH, file);
		_textures.Load(id, path);
		std::cout << id << " :: " << path << '\n';
	}

	//Load AirplaneData
	sol::table planes = level["usedAirplanes"];
	for (int i = 1; i <= planes.size(); i++)
	{
		std::string name = planes[i];
		std::string path = BuildString(AIRPLANES_PATH, name, ".lua");
		std::cout << path << '\n';

		sol::table plane = _context->lua->do_file(path);

		AirplaneData apdata;
		apdata.hitpoints = plane["hitpoints"];
		apdata.idleRect = TableToRect(plane["idleRect"]);
		apdata.rightRect = TableToRect(plane["rightRect"]);
		apdata.leftRect = TableToRect(plane["leftRect"]);
		apdata.texture = plane["texture"];
		apdata.speed = plane["speed"];

		sol::table directions = plane["aiPattern"];
		for (int i = 1; i <= directions.size(); i++)
		{
			AiDirection d;
			d.angle = directions[i][1];
			d.distance = directions[i][2];
			apdata.directions.push_back(d);
		}

		//Load weapons for airplane
		sol::table weapons = plane["weapons"];
		for (int i = 1; i < weapons.size(); i++)
		{
			sol::table weapon = weapons[i];
			std::string projectileName = weapon["projectile"];
			int ammo = weapon["ammo"];

			std::string projectilePath = BuildString(PROJECTILES_PATH, projectileName, ".lua");

			std::cout << projectilePath << '\n';
			sol::table projectile = _context->lua->do_file(projectilePath);
			ProjectileData projdata;
			projdata.texture = projectile["texture"];
			projdata.rect = TableToRect(projectile["rect"]);
			projdata.speed = projectile["speed"];
			projdata.damage = projectile["damage"];
			projdata.fireRate = projectile["fireRate"];
			projdata.onCollision = projectile["onCollision"];
			sol::function fixedUpdate = projectile["fixedUpdate"];
			sol::function update = projectile["update"];
			sol::function onDestroy = projectile["onDestroy"];
			if (fixedUpdate)
			{
				projdata.fixedUpdate = fixedUpdate;
			}
			if (update)
			{
				projdata.update = update;
			}
			if (onDestroy)
			{
				projdata.onDestroy = onDestroy;
			}

			_projectileDataDict.insert(std::make_pair(projectileName,
				projdata));
			apdata.weapons.push_back(&_projectileDataDict[projectileName]);
			apdata.ammo.push_back(ammo);
		}

		_airplaneDataDict.insert(std::make_pair(name,
			apdata));
	}

	for (auto pair : _airplaneDataDict)
	{
		std::cout << "FIRST: " << pair.first << ' ' << _airplaneDataDict["Eagle"].hitpoints << '\n';
	}
	auto& airplaneData = _airplaneDataDict["Eagle"];
	std::unique_ptr<Airplane> airplane(new Airplane(&airplaneData));
	airplane->SetPlayerControlled(true);
	airplane->setPosition(50, 50);
	_root->AddChild(std::move(airplane));

	_root->Start(this);
}


bool Level::FixedUpdate(float dt)
{
	_root->RemoveDestroyedChilldren();
	_root->FixedUpdate(dt);
	return false;
}


bool Level::Update(float dt)
{
	_root->Update(dt);
	return false;
}


bool Level::Render()
{
	_root->Draw(*_context->window, sf::RenderStates::Default);
	return false;
}