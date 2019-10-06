#include "Scenes/Level.h"
#include <string>
#include <iostream>
#include "Context.h"
#include <sol/sol.hpp>
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/SpriteObject.h"
#include "GameObjects/Projectile.h"
#include <sol/sol.hpp>
#include "Utility.h"
#include <vector>
#include <memory>
#include <algorithm>


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


	bool CheckCollision(const GameObject& lhs, const GameObject& rhs)
	{
		return lhs.GetBoundingRect().intersects(rhs.GetBoundingRect());
	}
}

Level::Level(Context* context, const std::string& fileName) :
	Scene(context),
	_root(new GameObject()),
	_fpsUpdateInterval(0.5f),
	_lastFpsUpdate(0.f)

{
	_worldView = _context->window->getDefaultView();

	if (_player.HasSettings())
	{
		_player.LoadSettings();
	}
	else
	{
		_player.SaveSettings();
	}


	std::cout << BuildString(LEVELS_PATH, fileName) << '\n';
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

	std::string backgroundTexture = level["backgroundTexture"];
	bool repeatBackground = level["repeatBackground"];
	_textures[backgroundTexture].setRepeated(repeatBackground);
	std::unique_ptr<SpriteObject> backgroundPtr(new SpriteObject());
	backgroundPtr->SetTexture(_textures[backgroundTexture]);

	_scrollSpeed = level["scrollSpeed"];
	_worldHeight = level["height"];

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
			projdata.muzzleRect = TableToRect(projectile["muzzleRect"]);
			projdata.speed = projectile["speed"];
			projdata.damage = projectile["damage"];
			projdata.fireRate = projectile["fireRate"];

			projdata.spreadAngle = projectile["spreadAngle"];
			projdata.generator = std::uniform_real_distribution<float>(-projdata.spreadAngle, projdata.spreadAngle);

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

	//Create player
	sol::table playerTable = level["player"];
	_playerSpawn.x = playerTable["spawnPoint"][1];
	_playerSpawn.y = playerTable["spawnPoint"][2];
	std::string playerPlane = playerTable["airplane"];

	_worldView.setCenter(_playerSpawn);

	auto& airplaneData = _airplaneDataDict[playerPlane];
	std::unique_ptr<Airplane> airplane(new Airplane(&airplaneData));
	airplane->SetPlayerControlled(true);
	airplane->setPosition(_playerSpawn);
	_playerAirplane = airplane.get();

	_enemyProjectilesRoot = new GameObject();
	_playerProjectilesRoot = new GameObject();
	_enemyAirplanesRoot = new GameObject();

	std::unique_ptr<GameObject> uptr1(_enemyProjectilesRoot);
	std::unique_ptr<GameObject> uptr2(_playerProjectilesRoot);
	std::unique_ptr<GameObject> uptr3(_enemyAirplanesRoot);

	_root->AddChild(std::move(backgroundPtr));
	_root->AddChild(std::move(uptr1));
	_root->AddChild(std::move(uptr2));
	_root->AddChild(std::move(uptr3));

	_root->AddChild(std::move(airplane));
	

	_root->Start(this);

	//Add Spawn Positions
	std::cout << "1\n";
	sol::table spawnPoints = level["spawnPoints"];
	for (int i = 1; i <= spawnPoints.size(); i++)
	{
		AirplaneSpawnPosition spawn;
		std::string id = spawnPoints[i][1];
		spawn.x = spawnPoints[i][2];
		spawn.y = spawnPoints[i][3];
		spawn.data = &_airplaneDataDict[id];
		std::cout << spawn.x << ' ' << spawn.y << '\n';
		_enemySpawns.push_back(spawn);
	}
	
	std::sort(_enemySpawns.begin(), _enemySpawns.end(), 
		[](const AirplaneSpawnPosition& lhs,
			const AirplaneSpawnPosition& rhs)
		{
			return lhs.y > rhs.y;
		});

	std::cout << "ORDER\n";
	for (auto it = _enemySpawns.begin(); it != _enemySpawns.end(); ++it)
	{
		std::cout << it->y << '\n';
	}
}
 

bool Level::FixedUpdate(float dt)
{
	_root->RemoveDestroyedChilldren();
	RemoveOffScreenObjects(dt);
	HandleCollisions(dt);
	SpawnEnemies();
	_player.HandleRealtimeInput(_commands);
	while (!_commands.IsEmpty())
	{
		_root->OnCommand(_commands.Pop(), dt);
	}
	_root->FixedUpdate(dt);
	_playerAirplane->move(0, -_scrollSpeed * dt);
	_worldView.move(0, -_scrollSpeed * dt);
	_root->RemoveDestroyedChilldren();
	return false;
}


bool Level::Update(float dt)
{
	_lastFpsUpdate += dt;
	if (_lastFpsUpdate > _fpsUpdateInterval)
	{
		int fps = (int)(1.f / dt);
		_context->window->setTitle(BuildString(fps));
		_lastFpsUpdate = 0.f;
	}
	_root->Update(dt);
	return false;
}

 
bool Level::Render()
{
	_context->window->setView(_worldView);
	_root->Draw(*_context->window, sf::RenderStates::Default);
	return true;
}


bool Level::HandleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		_context->window->close();
		break;

	case sf::Event::KeyPressed:
		_player.HandleEvent(ev, _commands);
		break;
	}
	
	return false;
}


void Level::SpawnEnemies()
{
	bool change = true;
	while (!_enemySpawns.empty() && change)
	{
		change = false;
		if (_enemySpawns[0].y > _worldView.getCenter().y - _worldView.getSize().y / 2 - 100)
		{
			std::unique_ptr<Airplane> airplanePtr(new Airplane(_enemySpawns[0].data));
			airplanePtr->setPosition(_enemySpawns[0].x, _enemySpawns[0].y);
			airplanePtr->SetPlayerControlled(false);
			airplanePtr->setRotation(180);
			airplanePtr->Start(this);
			_enemyAirplanesRoot->AddChild(std::move(airplanePtr));

			_enemySpawns.pop_front();
			change = true;
		}
	}
}


void Level::RemoveOffScreenObjects(float dt) 
{
	Command remover;
	remover.category = GameObject::EnemyAirplane | GameObject::EnemyProjectile |
		GameObject::PlayerProjectile;
	remover.action = [this](GameObject& obj, float dt)
	{
		if (obj.GetCategory() == GameObject::PlayerProjectile)
		{
			if (obj.GetWorldPosition().y < _worldView.getCenter().y - _worldView.getSize().y - 100)
			{
				std::cout << "Removed\n";
				obj.MarkForDestroy();
			}
		}
		else
		{
			if (obj.GetWorldPosition().y > _worldView.getCenter().y + _worldView.getSize().y + 100)
			{
				obj.MarkForDestroy();
			}
		}
	};

	_root->OnCommand(remover, dt);
}


void Level::HandleCollisions(float dt)
{
	sf::FloatRect playerRect = _playerAirplane->GetBoundingRect();
	Command enemyProjectileCollector;
	enemyProjectileCollector.category = GameObject::EnemyProjectile;
	enemyProjectileCollector.action = DeriveAction<Projectile>([this, playerRect](Projectile& proj, float dt)
	{
		if (proj.GetBoundingRect().intersects(playerRect))
		{
			proj.OnCollision(_playerAirplane);
		}
	});
	_enemyProjectilesRoot->OnCommand(enemyProjectileCollector, dt);

	Command enemyCollector;
	enemyCollector.category = GameObject::EnemyAirplane;
	enemyCollector.action = DeriveAction<Airplane>(
		[this, playerRect](Airplane& obj, float dt)
		{
			sf::FloatRect enemyRect = obj.GetBoundingRect();
			if (enemyRect.intersects(playerRect))
			{
				obj.MarkForDestroy();
				_playerAirplane->Damage(100);
			}
			else
			{
				Command playerProjectileCollector;
				playerProjectileCollector.category = GameObject::PlayerProjectile;
				playerProjectileCollector.action = DeriveAction<Projectile>(
					[this, enemyRect, &obj](Projectile& proj, float dt)
					{
						if (proj.GetBoundingRect().intersects(enemyRect))
						{
							proj.OnCollision(&obj);
						}
					}
				);

				_playerProjectilesRoot->OnCommand(playerProjectileCollector, dt);
			}
		}
	);
	_enemyAirplanesRoot->OnCommand(enemyCollector, dt);
}


void Level::AddPlayerProjectile(Projectile* proj)
{
	proj->Start(this);
	std::unique_ptr<Projectile> ptr(proj);
	_playerProjectilesRoot->AddChild(std::move(ptr));
}


void Level::AddEnemyProjectile(Projectile* proj)
{
	proj->Start(this);
	std::unique_ptr<Projectile> ptr(proj);
	_enemyProjectilesRoot->AddChild(std::move(ptr));
}


void Level::AddEnemyAirplane(Airplane* plane)
{
	plane->Start(this);
	std::unique_ptr<Airplane> ptr(plane);
}