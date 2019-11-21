#include "Scenes/Level.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Context.h"
#include <sol/sol.hpp>
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameObjects/Animation.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/SpriteObject.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/TextObject.h"
#include <sol/sol.hpp>
#include "Utility.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "Scenes/LoseState.h"
#include "Scenes/WinState.h"

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace rjs = rapidjson;


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

Level::Level(Context* context, const std::string& path) :
	Scene(context),
	_root(new GameObject()),
	_fpsUpdateInterval(0.5f),
	_lastFpsUpdate(0.f),
	_uiRoot(new GameObject),
	_currentText(nullptr),
	_localMenu(new LocalMenu(context)),
	_playingVignette(false),
	_vignetteCurrentIntensity(0.f),
	_nextIndex(0),
	_firstIndex(1),
	_highScore(0),
	_win(false)
	
{
	std::cout << "BEGIN_LEVEL_LOAD\n";
	std::random_device randDevice;
	_localMenu->SetLevel(this);
	_worldView = _context->window->getDefaultView();


	sol::table level = _context->lua->do_file(path);

	_saveFile = level["saveFile"];
	std::ifstream save(_saveFile);
	if (save.good())
	{
		rjs::IStreamWrapper wrapper(save);
		rjs::Document document;
		document.ParseStream(wrapper);
		_highScore = document["HighScore"].GetInt();
	}

	_defaultFont = level["defaultFont"];
	
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
	//bool repeatBackground = level["repeatBackground"];
	//_textures[backgroundTexture].setRepeated(repeatBackground);
	//std::unique_ptr<SpriteObject> backgroundPtr(new SpriteObject());
	_background.push_back(new SpriteObject());
	_background.push_back(new SpriteObject());
	//backgroundPtr->SetTexture(_textures[backgroundTexture]);
	
	_scrollSpeed = level["scrollSpeed"];
	_levelLength = level["length"];
	_scale = level["scale"];
	_borderSize = level["borderSize"];
	//backgroundPtr->setScale(_scale, _scale);
	_background[0]->setScale(_scale, _scale);
	_background[1]->setScale(_scale, _scale);
	_background[0]->SetTexture(_textures[backgroundTexture]);
	_background[1]->SetTexture(_textures[backgroundTexture]);

	//Load Pickups
	sol::table pickups = level["usedPickups"];
	for (int i = 1; i <= pickups.size(); i++)
	{
		std::string pickupName = pickups[i];
		std::string pickupPath = BuildString(PICKUPS_PATH, pickupName, ".lua");
		sol::table pickup = _context->lua->do_file(pickupPath);
		PickupData pickupData;
		pickupData.texture = &_textures[pickup["texture"]];
		pickupData.firstRect = TableToRect(pickup["firstRect"]);//pickup["firstRect"];
		pickupData.frames = pickup["frames"];
		pickupData.frameDuration = pickup["frameDuration"];
		pickupData.scale = pickup["scale"];
		pickupData.onPickup = pickup["onPickup"];
		sol::table destroyPickup = pickup["destroyAnimation"];
		pickupData.destroyTexture = &_textures[destroyPickup["texture"]];
		pickupData.destroyFirstRect = TableToRect(destroyPickup["firstRect"]);
		pickupData.destroyFrames = destroyPickup["frames"];
		pickupData.destroyFrameDuration = destroyPickup["frameDuration"];
		pickupData.destroyScale = destroyPickup["scale"];
		_pickupDataDict.insert(std::make_pair(pickupName, pickupData));
	}

	//Load AirplaneData
	sol::table planes = level["usedAirplanes"];
	for (int i = 1; i <= planes.size(); i++)
	{
		std::string name = planes[i];
		std::string path = BuildString(AIRPLANES_PATH, name, ".lua");
		std::cout << path << '\n';
		_enemiesDowned[name] = 0;

		sol::table plane = _context->lua->do_file(path);

		AirplaneData apdata;
		apdata.name = name;
		apdata.score = plane["score"];
		apdata.hitpoints = plane["hitpoints"];
		apdata.idleRect = TableToRect(plane["idleRect"]);
		apdata.rightRect = TableToRect(plane["rightRect"]);
		apdata.leftRect = TableToRect(plane["leftRect"]);
		apdata.texture = &_textures[plane["texture"]];
		apdata.speed = plane["speed"];
		apdata.healthFont = &_fonts[plane["healthFont"]];
		apdata.healthTextCharSize = plane["healthCharSize"];
		apdata.scale = plane["scale"];
		apdata.rng = std::mt19937(randDevice());
		apdata.dropRng = std::mt19937(randDevice());

		sol::table explosionData = plane["explosionData"];
		apdata.explosionSize = sf::Vector2i(explosionData["frameSize"][1], explosionData["frameSize"][2]);
		apdata.framesPerExplosion = explosionData["framesPerAnimation"];
		apdata.numberOfExplosions = explosionData["numberOfAnimations"];
		apdata.explosionFrameDuration = explosionData["frameDuration"];
		apdata.explosionsTexture = &_textures[explosionData["texture"]];
		apdata.explosionMaxScale = explosionData["maxScale"];
		apdata.explosionMinScale = explosionData["minScale"];
		apdata.explosionMaxRotation = explosionData["maxRotation"];
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
			projdata.name = projectileName;
			projdata.texture = &_textures[projectile["texture"]];
			projdata.iconTexture = &_textures[projectile["iconTexture"]];
			projdata.iconRect = TableToRect(projectile["iconRect"]);
			projdata.rect = TableToRect(projectile["rect"]);
			projdata.muzzleRect = TableToRect(projectile["muzzleRect"]);
			projdata.speed = projectile["speed"];
			projdata.damage = projectile["damage"];
			projdata.fireRate = projectile["fireRate"];
			projdata.scale = projectile["scale"];
			projdata.ammoFont = &_fonts[projectile["ammoFont"]];
			projdata.ammoTextSize = projectile["ammoTextSize"];
			projdata.muzzleScale = projectile["muzzleScale"];
			projdata.iconScale = projectile["iconScale"];
			projdata.start = projectile["start"];
			projdata.create = createProjectile;

			projdata.spreadAngle = projectile["spreadAngle"];
			projdata.rng = std::mt19937(randDevice());
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

		//Load drops
		sol::table drops = plane["drops"];
		for (int i = 1; i <= drops.size(); i++)
		{
			sol::table drop = drops[i];
			DropData dropData;
			dropData.dropRate = drop["dropRate"];
			std::string dropName = drop["pickup"];
			dropData.pickup = &_pickupDataDict[dropName];

			apdata.drops.push_back(dropData);
		}
		
		std::sort(apdata.drops.begin(), apdata.drops.end(), 
			[](const DropData& lhs, const DropData& rhs)
			{
				return lhs.dropRate < rhs.dropRate;
			});

		std::cout << "DROPCHANCE: " << apdata.drops[0].dropRate << '\n';
		for (int i = 1; i < apdata.drops.size(); i++)
		{
			apdata.drops[i].dropRate += apdata.drops[i - 1].dropRate;
			std::cout << "DROPCHANCE: " << apdata.drops[i].dropRate << '\n';
		}

		apdata.dropGenerator = std::uniform_int_distribution<int>(1, 100);

		_airplaneDataDict.insert(std::make_pair(name,
			apdata));
	}

	//Create player
	sol::table playerTable = level["player"];
	//_playerSpawn.x = playerTable["spawnPoint"][1];
	//_playerSpawn.y = playerTable["spawnPoint"][2];
	_playerSpawn.x = _context->window->getSize().x / 2;
	_playerSpawn.y = _levelLength - _worldView.getSize().y / 2;
	std::string playerPlane = playerTable["airplane"];

	_worldView.setCenter(_playerSpawn);

	auto& airplaneData = _airplaneDataDict[playerPlane];
	std::unique_ptr<Airplane> airplane(new Airplane(&airplaneData));
	airplane->SetPlayerControlled(true);
	airplane->setPosition(_playerSpawn);
	_playerAirplane = airplane.get();

	_background[1]->setPosition(0, _worldView.getCenter().y - _background[1]->GetBoundingRect().height + _worldView.getSize().y / 2);

	_enemyProjectilesRoot = new GameObject();
	_playerProjectilesRoot = new GameObject();
	_enemyAirplanesRoot = new GameObject();
	_explosionsRoot = new GameObject();
	_particlesRoot = new GameObject();
	_environmentRoot = new GameObject();
	_pickupsRoot = new GameObject();

	std::unique_ptr<GameObject> uptr1(_enemyProjectilesRoot);
	std::unique_ptr<GameObject> uptr2(_playerProjectilesRoot);
	std::unique_ptr<GameObject> uptr3(_enemyAirplanesRoot);
	std::unique_ptr<GameObject> uptr4(_explosionsRoot);
	std::unique_ptr<GameObject> uptr5(_particlesRoot);
	std::unique_ptr<GameObject> uptr6(_environmentRoot);
	std::unique_ptr<GameObject> uptr7(_pickupsRoot);
	std::unique_ptr<GameObject> uptr8(_background[1]);
	std::unique_ptr<GameObject> uptr9(_background[0]);


	_root->AddChild(std::move(uptr8));
	_root->AddChild(std::move(uptr9));
	_root->AddChild(std::move(uptr6));
	_root->AddChild(std::move(uptr4));
	_root->AddChild(std::move(uptr7));
	_root->AddChild(std::move(uptr1));
	_root->AddChild(std::move(uptr2));
	_root->AddChild(std::move(uptr3));
	_root->AddChild(std::move(uptr5));

	_root->AddChild(std::move(airplane));

	//_background[1]->setPosition(_playerSpawn);


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
		//animation->Start(this);
		//std::unique_ptr<Animation> animationPtr(animation);
		//_environmentRoot->AddChild(std::move(animationPtr));
		_environmentSpawns.push_back(animation);
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

	std::sort(_environmentSpawns.begin(), _environmentSpawns.end(),
		[](const Animation* lhs, 
			const Animation* rhs)
		{
			return lhs->GetWorldPosition().y > rhs->GetWorldPosition().y;
		});

	_flashShader.loadFromFile("assets/shaders/tint.frag", sf::Shader::Fragment);
	_vignetteShader.loadFromFile("assets/shaders/vignette.frag", sf::Shader::Fragment);
	_renderTexture.create(_context->window->getSize().x, _context->window->getSize().y);
	_vignetteShader.setUniform("u_resolution", sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));
	_root->Start(this);

	std::string soundtrack = level["soundtrack"];
	_context->music->openFromFile(soundtrack);
	_context->music->setVolume(_context->player->GetMusicVolume());
	_context->music->play();
}


Level::~Level()
{
	for (int i = 0; i < _environmentSpawns.size(); i++)
	{
		delete _environmentSpawns[i];
	}
}


bool Level::FixedUpdate(float dt)
{
	RemoveOffScreenObjects(dt);
	HandleCollisions(dt);
	CapPlayerPosition();
	if (_playerAirplane && _playerAirplane->IsDestroyed())
	{
		_playerAirplane = nullptr;
		std::shared_ptr<LoseState> lose(new LoseState(_context, this));
		RequestPush(lose);
	}
	else if (_playerAirplane && !_win && _worldView.getCenter().y < _playerSpawn.y - _levelLength)
	{
		std::shared_ptr<WinState> lose(new WinState(_context, this));
		RequestPush(lose);
		_win = true;
	}
	SpawnObjects();
	_context->player->HandleRealtimeInput(_commands);
	while (!_commands.IsEmpty())
	{
		_root->OnCommand(_commands.Pop(), dt);
	}
	_root->FixedUpdate(dt);

	if (_playerAirplane)
	{
		_playerAirplane->move(0, -_scrollSpeed * dt);
	}
	if (!_win)
	{
		_worldView.move(0, -_scrollSpeed * dt);
	}
	if (_worldView.getCenter().y < _background[_firstIndex]->getPosition().y + _background[_firstIndex]->GetBoundingRect().height / 2)
	{
		SwitchBackground();
	}
	DisplayText();
	_root->RemoveDestroyedChilldren();
	_uiRoot->RemoveDestroyedChilldren();

	return false; 
}


void Level::CapPlayerPosition()
{
	if (!_playerAirplane)
	{
		return;
	}

	auto viewBounds = GetViewBounds();
	auto playerPosition = _playerAirplane->getPosition();
	float leftBorder = viewBounds.left + _borderSize;
	float rightBorder = viewBounds.left + viewBounds.width - _borderSize;
	float topBorder = viewBounds.top + _borderSize;
	float bottomBorder = viewBounds.top + viewBounds.height - _borderSize;

	playerPosition.x = playerPosition.x < leftBorder ? leftBorder : playerPosition.x;
	playerPosition.x = playerPosition.x > rightBorder ? rightBorder : playerPosition.x;
	playerPosition.y = playerPosition.y < topBorder ? topBorder : playerPosition.y;
	playerPosition.y = playerPosition.y > bottomBorder ? bottomBorder : playerPosition.y;
	_playerAirplane->setPosition(playerPosition);
}


sf::FloatRect Level::GetViewBounds() const
{
	return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
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

	if (_playingVignette)
	{
		_vignetteElapsedTime += dt;
		float progress = _vignetteElapsedTime / _vignetteDuration;
		if (_vignetteElapsedTime > _vignetteDuration)
		{
			_playingVignette = false;
			_vignetteCurrentIntensity = 0.f;
		}
		else if (progress < _vignetteFadeInRatio)
		{
			_vignetteCurrentIntensity = _vignetteMaxIntensity * progress / _vignetteFadeInRatio;
		}
		else if (progress > 1.f - _vignetteFadeOutRatio)
		{
			_vignetteCurrentIntensity = _vignetteMaxIntensity * (1.f - progress) / _vignetteFadeOutRatio;
		}
	}

	_root->Update(dt);
	_uiRoot->Update(dt);
	return false;
}

 
bool Level::Render()
{
	//_context->window->setView(_worldView);
	_context->window->setView(_context->window->getDefaultView());
	_renderTexture.clear();
	_renderTexture.setView(_worldView);
	_root->Draw(_renderTexture, sf::RenderStates::Default);
	_renderTexture.display();

	sf::Sprite sprite(_renderTexture.getTexture());
	sf::RenderStates renderStates = sf::RenderStates::Default;
	
	_vignetteShader.setUniform("u_texture", _renderTexture.getTexture());
	if (!_win)
	{
		_vignetteShader.setUniform("u_intensity", _vignetteCurrentIntensity);
	}
	_context->window->draw(sprite, &_vignetteShader);

	
	_context->window->setView(_worldView);
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
		_context->player->HandleEvent(ev, _commands);
		if (ev.key.code == sf::Keyboard::Escape)
		{
			RequestPush(_localMenu);
		}
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


void Level::SpawnObjects()
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

	change = true;
	while (!_environmentSpawns.empty() && change)
	{
		change = false;
		if (_environmentSpawns[0]->getPosition().y > _worldView.getCenter().y - _worldView.getSize().y / 2 - 100)
		{
			std::unique_ptr<Animation> obj(_environmentSpawns[0]);
			obj->Start(this);
			_environmentRoot->AddChild(std::move(obj));
			_environmentSpawns.pop_front();
			change = true;
		}
	}
}


void Level::RemoveOffScreenObjects(float dt) 
{
	Command remover;
	auto viewSize = _worldView.getSize();
	auto viewCenter = _worldView.getCenter();
	remover.category = GameObject::EnemyAirplane | GameObject::EnemyProjectile |
		GameObject::PlayerProjectile | GameObject::PickupItem;
	remover.action = [this, viewSize, viewCenter](GameObject& obj, float dt)
	{
		auto objPosition = obj.GetWorldPosition();

		if (obj.GetCategory() == GameObject::PlayerProjectile)
		{
			if (objPosition.y < viewCenter.y - viewSize.y ||
				objPosition.y > viewCenter.y + viewSize.y ||
				objPosition.x > viewCenter.x + viewSize.x ||
				objPosition.x < viewCenter.x - viewSize.x)
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
	Command envRemover;
	envRemover.category = GameObject::AnimationType;
	envRemover.action = [this, viewSize, viewCenter](GameObject& obj, float dt)
	{
		if (obj.GetWorldPosition().y > viewCenter.y + viewSize.y + 100)
		{
			obj.MarkForDestroy();
		}
	};
	_environmentRoot->OnCommand(envRemover, dt);
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

	Command pickupCollector;
	pickupCollector.category = GameObject::PickupItem;
	pickupCollector.action = DeriveAction<Pickup>(
		[this, playerRect](Pickup& pickup, float dt)
		{
			if (pickup.GetBoundingRect().intersects(playerRect))
			{
				if (!pickup.WasUsed())
				{
					pickup.OnPickup(*_playerAirplane);
				}
			}
		});
	_pickupsRoot->OnCommand(pickupCollector, dt);

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


void Level::AddPickup(Pickup* pickup)
{
	pickup->Start(this);
	std::unique_ptr<Pickup> ptr(pickup);
	_pickupsRoot->AddChild(std::move(ptr));  
}


void Level::PlayVignetteAnimation(const sf::Glsl::Vec4& color, float inner, float outer, float intensity, float fadeInRatio, float fadeOutRatio, float duration)
{
	_vignetteCurrentIntensity = 0.f;
	_vignetteMaxIntensity = intensity;
	_vignetteElapsedTime = 0.f;
	_vignetteFadeInRatio = fadeInRatio;
	_vignetteFadeOutRatio = fadeOutRatio;
	_vignetteColor = color;
	_vignetteInnerRadius = inner;
	_vignetteOuterRadius = outer;
	_vignetteDuration = duration;
	_playingVignette = true;

	_vignetteShader.setUniform("u_vignetteColor", _vignetteColor);
	_vignetteShader.setUniform("u_innerRadius", _vignetteInnerRadius);
	_vignetteShader.setUniform("u_outerRadius", _vignetteOuterRadius);
	_vignetteShader.setUniform("u_resolution", sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));
}


void Level::SwitchBackground()
{
	_background[_nextIndex]->setPosition(0, _background[_firstIndex]->getPosition().y - _background[_firstIndex]->GetBoundingRect().height);

	int aux = _firstIndex;
	_firstIndex = _nextIndex;
	_nextIndex = aux;
}