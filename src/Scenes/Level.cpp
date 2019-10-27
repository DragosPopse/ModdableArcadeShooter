#include "Scenes/Level.h"
#include <string>
#include <iostream>
#include "Context.h"
#include <sol/sol.hpp>
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameObjects/Animation.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/SpriteObject.h"
#include "GameObjects/Projectile.h"
#include <sol/sol.hpp>
#include "Utility.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Audio.hpp>


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
	_lastFpsUpdate(0.f),
	_uiRoot(new GameObject),
	_currentText(nullptr)
	
{
	std::cout << "BEGIN_LEVEL_LOAD\n";

	_worldView = _context->window->getDefaultView();

	if (_player.HasSettings())
	{
		std::cout << "LOAD_SETTINGS\n";
		_player.LoadSettings();
		std::cout << "LOADED_SETTINGS\n";
	}
	else
	{
		_player.SaveSettings();
	}


	std::cout << BuildString(LEVELS_PATH, fileName) << '\n';
	sol::table level = _context->lua->do_file(BuildString(LEVELS_PATH, fileName));
	
	//Load required textures
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

	//Load required fonts
	sol::table usedFonts = level["usedFonts"];
	for (int i = 1; i <= usedFonts.size(); i++)
	{
		sol::table font = usedFonts[i];
		std::string id = font[1];
		std::string file = font[2];
		std::string path = BuildString(FONTS_PATH, file);
		_fonts.Load(id, path);
		std::cout << id << " :: " << path << '\n';
	}

	std::string backgroundTexture = level["backgroundTexture"];
	bool repeatBackground = level["repeatBackground"];
	_textures[backgroundTexture].setRepeated(repeatBackground);
	std::unique_ptr<SpriteObject> backgroundPtr(new SpriteObject());
	backgroundPtr->SetTexture(_textures[backgroundTexture]);

	_scrollSpeed = level["scrollSpeed"];
	_worldHeight = level["height"];
	_scale = level["scale"];
	backgroundPtr->setScale(_scale, _scale);
	backgroundPtr->SetTextureRect(sf::IntRect(0, 0, _textures[backgroundTexture].getSize().x, _worldHeight));

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
		apdata.healthFont = plane["healthFont"];
		apdata.healthTextCharSize = plane["healthCharSize"];
		apdata.scale = plane["scale"];

		sol::table explosionData = plane["explosionData"];
		apdata.explosionSize = sf::Vector2i(explosionData["frameSize"][1], explosionData["frameSize"][2]);
		apdata.framesPerExplosion = explosionData["framesPerAnimation"];
		apdata.numberOfExplosions = explosionData["numberOfAnimations"];
		apdata.explosionFrameDuration = explosionData["frameDuration"];
		apdata.explosionsTexture = explosionData["texture"];
		apdata.explosionMaxScale = explosionData["maxScale"];
		apdata.explosionMinScale = explosionData["minScale"];
		apdata.generator = std::uniform_real_distribution<float>(apdata.explosionMinScale, apdata.explosionMaxScale);


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
		for (int i = 1; i <= weapons.size(); i++)
		{
			sol::table weapon = weapons[i];
			std::string projectileName = weapon["projectile"];
			if (_projectileDataDict.find(projectileName) != _projectileDataDict.end())
			{
				continue; //Don't load projectiles multiple times 
			}
	/*		if (projectileName == "HomingMissile")
			{
				std::cout << "break\n";
			}*/
			int ammo = weapon["ammo"];

			std::string projectilePath = BuildString(PROJECTILES_PATH, projectileName, ".lua");

			std::cout << projectilePath << '\n';
			sol::table projectile;
			sol::function createProjectile;
			sol::tie(projectile, createProjectile) = _context->lua->do_file(projectilePath);
			ProjectileData projdata;
			projdata.texture = projectile["texture"];
			projdata.iconTexture = projectile["iconTexture"];
			projdata.iconRect = TableToRect(projectile["iconRect"]);
			projdata.rect = TableToRect(projectile["rect"]);
			projdata.muzzleRect = TableToRect(projectile["muzzleRect"]);
			projdata.speed = projectile["speed"];
			projdata.damage = projectile["damage"];
			projdata.fireRate = projectile["fireRate"];
			projdata.scale = projectile["scale"];
			projdata.muzzleScale = projectile["muzzleScale"];
			projdata.iconScale = projectile["iconScale"];
			projdata.start = projectile["start"];
			projdata.create = createProjectile;

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

	_worldView.setCenter(400, _playerSpawn.y);

	auto& airplaneData = _airplaneDataDict[playerPlane];
	std::unique_ptr<Airplane> airplane(new Airplane(&airplaneData));
	airplane->SetPlayerControlled(true);
	airplane->setPosition(_playerSpawn);
	_playerAirplane = airplane.get();

	_enemyProjectilesRoot = new GameObject();
	_playerProjectilesRoot = new GameObject();
	_enemyAirplanesRoot = new GameObject();
	_explosionsRoot = new GameObject();
	_particlesRoot = new GameObject();
	_environmentRoot = new GameObject();

	std::unique_ptr<GameObject> uptr1(_enemyProjectilesRoot);
	std::unique_ptr<GameObject> uptr2(_playerProjectilesRoot);
	std::unique_ptr<GameObject> uptr3(_enemyAirplanesRoot);
	std::unique_ptr<GameObject> uptr4(_explosionsRoot);
	std::unique_ptr<GameObject> uptr5(_particlesRoot);
	std::unique_ptr<GameObject> uptr6(_environmentRoot);


	_root->AddChild(std::move(backgroundPtr));
	_root->AddChild(std::move(uptr6));
	_root->AddChild(std::move(uptr4));
	_root->AddChild(std::move(uptr1));
	_root->AddChild(std::move(uptr2));
	_root->AddChild(std::move(uptr3));
	_root->AddChild(std::move(uptr5));

	_root->AddChild(std::move(airplane));


	//Add environment animations and sprites 
	sol::table animations = level["animations"];
	for (int i = 1; i <= animations.size(); i++)
	{
		sol::table animationData = animations[i];
		std::string animationTexture = animationData["texture"];
		int animationFrames = animationData["frames"];
		sf::IntRect animationFirstRect = TableToRect(animationData["firstRect"]);
		float animationFrameDuration = animationData["frameDuration"];
		sf::Vector2f animationPosition;
		animationPosition.x = animationData["position"][1];
		animationPosition.y = animationData["position"][2];

		Animation* animation = new Animation();
		animation->SetTexture(_textures[animationTexture]);
		animation->SetFirstRect(animationFirstRect);
		animation->SetNumberOfFrames(animationFrames);
		animation->SetLoopable(true);
		animation->SetTimePerFrame(animationFrameDuration);
		animation->SetDestroyOnFinish(false);
		animation->setPosition(animationPosition);
		animation->setScale(_scale, _scale);
		animation->Start(this);
		std::unique_ptr<Animation> animationPtr(animation);
		_environmentRoot->AddChild(std::move(animationPtr));
	}


	//Add Spawn Positions
	sol::table spawnPoints = level["spawnPoints"];
	for (int i = 1; i <= spawnPoints.size(); i++)
	{
		AirplaneSpawnPosition spawn;
		std::string id = spawnPoints[i][1];
		spawn.x = spawnPoints[i][2];
		spawn.y = spawnPoints[i][3];
		spawn.data = &_airplaneDataDict[id];
		_enemySpawns.push_back(spawn);
	}

	//Add texts
	sol::table texts = level["texts"];
	for (int i = 1; i <= texts.size(); i++)
	{
		TextAnimationData tdata;
		sol::table text = texts[1];
		tdata.str = text["text"];
		tdata.y = text["y"];
		tdata.timePerLetter = text["timePerLetter"];
		tdata.timeBeforeDestroy = text["timeBeforeDestroy"];
		tdata.charSize = text["charSize"];
		tdata.font = text["font"];
		_texts.push_back(tdata);
	}
	
	std::sort(_enemySpawns.begin(), _enemySpawns.end(), 
		[](const AirplaneSpawnPosition& lhs,
			const AirplaneSpawnPosition& rhs)
		{
			return lhs.y > rhs.y;
		});

	std::sort(_texts.begin(), _texts.end(),
		[](const TextAnimationData& lhs,
			const TextAnimationData& rhs)
		{
			return lhs.y > rhs.y;
		});

	std::cout << "LOAD_SHADER\n";
	_flashShader.loadFromFile("assets/shaders/tint.frag", sf::Shader::Fragment);
	std::cout << "LOADED_SHADER\n";

	_root->Start(this);
}
 

bool Level::FixedUpdate(float dt)
{
	RemoveOffScreenObjects(dt);
	HandleCollisions(dt);
	if (_playerAirplane && _playerAirplane->IsDestroyed())
	{
		_playerAirplane = nullptr;
	}
	SpawnEnemies();
	_player.HandleRealtimeInput(_commands);
	while (!_commands.IsEmpty())
	{
		_root->OnCommand(_commands.Pop(), dt);
	}
	_root->FixedUpdate(dt);
	if (_playerAirplane)
	{
		_playerAirplane->move(0, -_scrollSpeed * dt);
	}
	_worldView.move(0, -_scrollSpeed * dt);
	DisplayText();
	_root->RemoveDestroyedChilldren();
	_uiRoot->RemoveDestroyedChilldren();
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
	_uiRoot->Update(dt);
	return false;
}

 
bool Level::Render()
{
	_context->window->setView(_worldView);
	_root->Draw(*_context->window, sf::RenderStates::Default);
	
	sf::RenderStates uiStates = sf::RenderStates::Default;
	_uiRoot->setPosition(_worldView.getCenter());
	_uiRoot->Draw(*_context->window, uiStates);
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


void Level::DisplayText()
{
	bool change = true;
	if (_currentText && _currentText->IsDestroyed())
	{
		_currentText = nullptr;
	}
	while (!_texts.empty() && change)
	{
		change = false;
		if (_texts[0].y > _worldView.getCenter().y)
		{
			if (_currentText != nullptr)
			{
				_currentText->MarkForDestroy();
			}
			_currentText = new TextAnimation(_texts[0].str);
			_currentText->SetCharSize(_texts[0].charSize);
			_currentText->SetTimePerLetter(_texts[0].timePerLetter);
			_currentText->SetDestroyOnFinish(_texts[0].timeBeforeDestroy);
			_currentText->SetFont(_texts[0].font);
			_currentText->Start(this);
			std::unique_ptr<TextAnimation> ptr(_currentText);
			//_currentText->setPosition(_worldView.getSize().x / 2, _worldView.getSize().y / 2);
			_uiRoot->AddChild(std::move(ptr));
			change = true;
			_texts.pop_front();
		}
	}
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
		auto objPosition = obj.GetWorldPosition();
		auto viewSize = _worldView.getSize();
		auto viewCenter = _worldView.getCenter();
		if (obj.GetCategory() == GameObject::PlayerProjectile)
		{
			
			if (objPosition.y < viewCenter.y - viewSize.y ||
				objPosition.y > viewCenter.y + viewSize.y ||
				objPosition.x > viewCenter.x + viewSize.x ||
				objPosition.y < viewCenter.x - viewSize.x)
			{
				obj.MarkForDestroy();
			}
		}
		else
		{
			if (objPosition.y > viewCenter.y + viewSize.y + 100)
			{
				obj.MarkForDestroy();
			}
		}
	};

	_root->OnCommand(remover, dt);
}


void Level::HandleCollisions(float dt)
{
	if (!_playerAirplane)
	{
		return;
	}
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
				obj.Damage(99999);
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
	_enemyAirplanesRoot->AddChild(std::move(ptr));
}


void Level::AddExplosion(Animation* explosion)
{
	explosion->Start(this);
	std::unique_ptr<Animation> ptr(explosion);
	_explosionsRoot->AddChild(std::move(ptr));
}


void Level::AddParticles(ParticleSystemObject* p)
{
	//p->Start(this);
	//std::unique_ptr<ParticleSystemObject> ptr(p);
	//_particlesRoot->AddUnownedChild(p);
}


void Level::AddUiElement(GameObject* ui)
{
	ui->Start(this);
	std::unique_ptr<GameObject> ptr(ui);
	_uiRoot->AddChild(std::move(ptr));
}


void Level::AddLuaParticle(sol::userdata p)
{
	p.as<ParticleSystemObject>().Start(this);
	_particlesRoot->AddUnownedChild(p);
}