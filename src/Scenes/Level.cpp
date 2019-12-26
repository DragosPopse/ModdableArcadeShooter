#include "Scenes/Level.h"

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Scenes/LoseState.h"
#include "Scenes/WinState.h"
#include "Scenes/ErrorScene.h"
#include "Scenes/MainMenu.h"

#include "GameObject.h"
#include "GameObjects/Animation.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/SpriteObject.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/TextObject.h"


#include "Context.h"
#include "Engine.h"
#include "RandomizedSound.h"
#include "Utility.h"


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
	_highScore(0),
	_win(false),
	_currentBgIndex(0),
	_timeScale(1.f)
{
	auto tableToSound = [this](const sol::table& table) {
		RandomizedSound sound;
		sf::SoundBuffer* buffer = &this->_sounds[table["sound"]];
		sound.SetBuffer(buffer);
		sound.SetVolumeFactorDistribution(table["minVolumeFactor"], table["maxVolumeFactor"]);
		sound.SetPitchDistribution(table["minPitch"], table["maxPitch"]);
		return sound;
	};
	std::cout << "BEGIN_LEVEL_LOAD\n";
	_localMenu->SetLevel(this);
	_worldView = _context->window->getDefaultView();
	_shaker.SetView(&_worldView);
	sol::table level = Protect<sol::table>(_context->lua->do_file(path));
	_saveFile = Protect<std::string>(level["saveFile"]);
	std::ifstream save(_saveFile);
	if (save.good())
	{
		rjs::IStreamWrapper wrapper(save);
		rjs::Document document;
		document.ParseStream(wrapper);
		_highScore = document["HighScore"].GetInt();
	}

	_defaultFont = Protect<std::string>(level["defaultFont"]);
	
	//Load required textures
	sol::table usedTextures = Protect<sol::table>(level["usedTextures"]);
	for (int i = 1; i <= usedTextures.size(); i++)
	{
		sol::table texture = usedTextures[i];
		std::string id = Protect<std::string>(texture[1]);
		std::string file = Protect<std::string>(texture[2]);
		_textures.Load(id, file);
		std::cout << id << " :: " << path << '\n';
	}

	//Load required fonts
	sol::table usedFonts = Protect<sol::table>(level["usedFonts"]);
	for (int i = 1; i <= usedFonts.size(); i++)
	{
		sol::table font = Protect<sol::table>(usedFonts[i]);
		std::string id = Protect<std::string>(font[1]);
		std::string file = Protect<std::string>(font[2]);
		_fonts.Load(id, file);
	}

	//Load required sounds
	sol::table usedSounds = level["usedSounds"];
	for (int i = 1; i <= usedSounds.size(); i++)
	{
		sol::table sound = Protect<sol::table>(usedSounds[i]);
		std::string id = Protect<std::string>(sound[1]);
		std::string file = Protect<std::string>(sound[2]);
		_sounds.Load(id, file);
	}

	std::string backgroundTexture = Protect<std::string>(level["backgroundTexture"]);
	//bool repeatBackground = level["repeatBackground"];
	//_textures[backgroundTexture].setRepeated(repeatBackground);
	//std::unique_ptr<SpriteObject> backgroundPtr(new SpriteObject());
	_background.push_back(new SpriteObject());
	_background.push_back(new SpriteObject());
	//backgroundPtr->SetTexture(_textures[backgroundTexture]);
	
	_scrollSpeed = Protect<float>(level["scrollSpeed"]);
	_levelLength = Protect<float>(level["length"]);
	_scale = Protect<float>(level["scale"]);
	_borderSize = Protect<float>(level["borderSize"]);
	//backgroundPtr->setScale(_scale, _scale);
	_background[0]->setScale(_scale, _scale);
	_background[1]->setScale(_scale, _scale);
	_background[0]->SetTexture(_textures[backgroundTexture]);
	_background[1]->SetTexture(_textures[backgroundTexture]);
	
	//Load Pickups
	sol::table pickups = Protect<sol::table>(level["usedPickups"]);
	for (int i = 1; i <= pickups.size(); i++)
	{
		std::string pickupName = Protect<std::string>(pickups[i]);
		std::string pickupPath = BuildString("assets/scripts/pickups/", pickupName, ".lua");
		sol::table pickup = Protect<sol::table>(_context->lua->do_file(pickupPath));
		PickupData pickupData;
		pickupData.texture = &_textures[Protect<std::string>(pickup["texture"])];
		pickupData.firstRect = TableToRect(Protect<sol::table>(pickup["firstRect"]));//pickup["firstRect"];
		pickupData.frames = Protect<int>(pickup["frames"]);
		pickupData.frameDuration = Protect<float>(pickup["frameDuration"]);
		pickupData.scale = Protect<float>(pickup["scale"]);
		pickupData.onPickup = Protect<sol::function>(pickup["onPickup"]);
		sol::table destroyPickup = Protect<sol::table>(pickup["destroyAnimation"]);
		pickupData.destroyTexture = &_textures[Protect<std::string>(destroyPickup["texture"])];
		pickupData.destroyFirstRect = TableToRect(Protect<sol::table>(destroyPickup["firstRect"]));
		pickupData.destroyFrames = Protect<int>(destroyPickup["frames"]);
		pickupData.destroyFrameDuration = Protect<float>(destroyPickup["frameDuration"]);
		pickupData.destroyScale = Protect<float>(destroyPickup["scale"]);
		pickupData.onPickupSound = tableToSound(Protect<sol::table>(pickup["onPickupSound"]));
		
		_pickupDataDict.insert(std::make_pair(pickupName, pickupData));
	}

	//Load AirplaneData
	sol::table planes = Protect<sol::table>(level["usedAirplanes"]);
	for (int i = 1; i <= planes.size(); i++)
	{
		std::string name = Protect<std::string>(planes[i]);
		std::string path = BuildString("assets/scripts/airplanes/", name, ".lua");
		std::cout << path << '\n';
		sol::table plane = Protect<sol::table>(_context->lua->do_file(path));

		AirplaneData apdata;
		apdata.name = Protect<std::string>(plane["name"]);
		_enemiesDowned[apdata.name] = 0;
		apdata.score = Protect<int>(plane["score"]);
		apdata.hitpoints = Protect<int>(plane["hitpoints"]);
		apdata.idleRect = TableToRect(Protect<sol::table>(plane["idleRect"]));
		apdata.rightRect = TableToRect(Protect<sol::table>(plane["rightRect"]));
		apdata.leftRect = TableToRect(Protect<sol::table>(plane["leftRect"]));
		apdata.texture = &_textures[Protect<std::string>(plane["texture"])];
		apdata.speed = Protect<float>(plane["speed"]);
		apdata.healthFont = &_fonts[Protect<std::string>(plane["healthFont"])];
		apdata.healthTextCharSize = Protect<int>(plane["healthCharSize"]);
		apdata.scale = Protect<float>(plane["scale"]);
		sol::optional<sol::function> onDestroy = plane["onDestroy"];

		if (onDestroy)
		{
			apdata.onDestroy = onDestroy;
		}

		sol::table explosionData = Protect<sol::table>(plane["explosionData"]);
		sol::table explosionSizeTable = Protect<sol::table>(explosionData["frameSize"]);
		int explosionSizeX = Protect<int>(explosionSizeTable[1]);
		int explosionSizeY = Protect<int>(explosionSizeTable[2]);
		apdata.explosionSize = sf::Vector2i(explosionSizeX, explosionSizeY);
		apdata.framesPerExplosion = Protect<int>(explosionData["framesPerAnimation"]);
		apdata.numberOfExplosions = Protect<int>(explosionData["numberOfAnimations"]);
		apdata.explosionFrameDuration = Protect<float>(explosionData["frameDuration"]);
		apdata.explosionsTexture = &_textures[Protect<std::string>(explosionData["texture"])];
		apdata.explosionMaxScale = Protect<float>(explosionData["maxScale"]);
		apdata.explosionMinScale = Protect<float>(explosionData["minScale"]);
		apdata.explosionMaxRotation = Protect<float>(explosionData["maxRotation"]);
		sol::table explosionSounds = Protect<sol::table>(explosionData["sounds"]);
		apdata.switchSound = tableToSound(Protect<sol::table>(plane["switchSound"]));
		for (int i = 1; i <= explosionSounds.size(); i++)
		{
			RandomizedSound explosionSound = tableToSound(Protect<sol::table>(explosionSounds[i]));
			apdata.explosionSounds.push_back(explosionSound);
		}

		

		sol::table directions = Protect<sol::table>(plane["aiPattern"]);
		for (int i = 1; i <= directions.size(); i++)
		{
			AiDirection d;
			d.angle = Protect<float>(directions[i][1]);
			d.distance = Protect<float>(directions[i][2]);
			apdata.directions.push_back(d);
		}

		//Load weapons for airplane
		sol::object optionalWeapons = plane["weapons"];
		if (optionalWeapons != sol::nil)
		{
			sol::table weapons = optionalWeapons;
			for (int i = 1; i <= weapons.size(); i++)
			{
				sol::table weapon = Protect<sol::table>(weapons[i]);
				std::string projectileName = Protect<std::string>(weapon["projectile"]);
				if (_projectileDataDict.find(projectileName) != _projectileDataDict.end())
				{
					std::cout << "AIRPLANE: " << name << ' ' << projectileName << '\n';
					apdata.weapons.push_back(&_projectileDataDict[projectileName]);
					int ammo = Protect<int>(weapon["ammo"]);
					apdata.ammo.push_back(ammo);
					continue; //Don't load projectiles multiple times 
				}
				/*		if (projectileName == "HomingMissile")
						{
							std::cout << "break\n";
						}*/
				int ammo = Protect<int>(weapon["ammo"]);

				std::string projectilePath = BuildString("assets/scripts/projectiles/", projectileName, ".lua");

				std::cout << projectilePath << '\n';
				sol::table projectile = Protect<sol::table>(_context->lua->do_file(projectilePath));
	
				ProjectileData projdata;
				projdata.name = projectileName;
				projdata.texture = &_textures[Protect<std::string>(projectile["texture"])];
				projdata.iconTexture = &_textures[Protect<std::string>(projectile["iconTexture"])];
				projdata.muzzleSound = tableToSound(Protect<sol::table>(projectile["muzzleSound"]));
				projdata.destroySound = tableToSound(Protect<sol::table>(projectile["destroySound"]));
				projdata.iconRect = TableToRect(Protect<sol::table>(projectile["iconRect"]));
				sol::table rects = Protect<sol::table>(projectile["rects"]);
				for (int i = 1; i <= rects.size(); i++)
				{
					projdata.rects.push_back(TableToRect(Protect<sol::table>(rects[i])));
				}
				projdata.muzzleRect = TableToRect(Protect<sol::table>(projectile["muzzleRect"]));
				projdata.speed = Protect<float>(projectile["speed"]);
				projdata.damage = Protect<int>(projectile["damage"]);
				projdata.fireRate = Protect<float>(projectile["fireRate"]);
				projdata.scale = Protect<float>(projectile["scale"]);
				projdata.ammoFont = &_fonts[Protect<std::string>(projectile["ammoFont"])];
				projdata.ammoTextSize = Protect<int>(projectile["ammoTextSize"]);
				projdata.muzzleScale = Protect<float>(projectile["muzzleScale"]);
				projdata.iconScale = Protect<float>(projectile["iconScale"]);
				projdata.start = Protect<sol::function>(projectile["start"]);


				projdata.muzzleSound = tableToSound(Protect<sol::table>(projectile["muzzleSound"]));
				projdata.destroySound = tableToSound(Protect<sol::table>(projectile["destroySound"]));

				projdata.spreadAngle = Protect<float>(projectile["spreadAngle"]);
				
				sol::function onCollision = projectile["onCollision"];	
				sol::function fixedUpdate = projectile["fixedUpdate"];
				sol::function update = projectile["update"];
				sol::function onDestroy = projectile["onDestroy"];

				if (onCollision)
				{
					projdata.onCollision = onCollision;
				}
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

		}
		//Load drops
		sol::object optionalDrops = plane["drops"];
		if (optionalDrops != sol::nil)
		{
			sol::table drops = Protect<sol::table>(plane["drops"]);
			int percentage = 0;
			for (int i = 1; i <= drops.size(); i++)
			{
				sol::table drop = drops[i];
				DropData dropData;
				dropData.dropRate = Protect<int>(drop["dropRate"]);
				std::string dropName = Protect<std::string>(drop["pickup"]);
				dropData.pickup = &_pickupDataDict[dropName];

				apdata.drops.push_back(dropData);
				percentage += dropData.dropRate;
				if (dropData.dropRate < 0)
				{
					throw sol::error(BuildString(name, ".lua: dropRate cannot be negative. "));
				}
			}

			if (percentage > 100)
			{
				throw sol::error(BuildString(name, ".lua: drop rates exceed 100. The sum of all dropRates should be 100. "));
			}
		}
		
		std::sort(apdata.drops.begin(), apdata.drops.end(), 
			[](const DropData& lhs, const DropData& rhs)
			{
				return lhs.dropRate < rhs.dropRate;
			});

		for (int i = 1; i < apdata.drops.size(); i++)
		{
			apdata.drops[i].dropRate += apdata.drops[i - 1].dropRate;
			std::cout << "DROPCHANCE: " << apdata.drops[i].dropRate << '\n';
		}

		_airplaneDataDict.insert(std::make_pair(name,
			apdata));
	}

	//Create player
	sol::table playerTable = Protect<sol::table>(level["player"]);
	//_playerSpawn.x = playerTable["spawnPoint"][1];
	//_playerSpawn.y = playerTable["spawnPoint"][2];
	_playerSpawn.x = _context->window->getSize().x / 2;
	_playerSpawn.y = _levelLength - _worldView.getSize().y / 2;
	std::string playerPlane = Protect<std::string>(playerTable["airplane"]);

	_worldView.setCenter(_playerSpawn);
	_actualViewPosition = _worldView.getCenter().y;

	auto& airplaneData = _airplaneDataDict[playerPlane];
	std::unique_ptr<Airplane> airplane(new Airplane(&airplaneData));
	airplane->SetPlayerControlled(true);
	airplane->setPosition(_playerSpawn);
	_playerAirplane = airplane.get();

	//_background[1]->setPosition(0, _worldView.getCenter().y - _background[1]->GetBoundingRect().height + _worldView.getSize().y / 2);
	_background[0]->setPosition(_worldView.getCenter().x, _worldView.getCenter().y - _background[0]->GetBoundingRect().height / 2 + _worldView.getSize().y / 2);
	_background[1]->setPosition(_worldView.getCenter().x, _background[0]->getPosition().y - _background[0]->GetBoundingRect().height);

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
	sol::table animations = Protect<sol::table>(level["animations"]);
	for (int i = 1; i <= animations.size(); i++)
	{
		sol::table animationData = animations[i];
		std::string animationTexture = Protect<std::string>(animationData["texture"]);
		int animationFrames = Protect<int>(animationData["frames"]);
		sf::IntRect animationFirstRect = TableToRect(Protect<sol::table>(animationData["firstRect"]));
		float animationFrameDuration = Protect<float>(animationData["frameDuration"]);
		sf::Vector2f animationPosition;
		sol::table animationPositionTable = Protect<sol::table>(animationData["position"]);
		animationPosition.x = Protect<float>(animationPositionTable[1]);
		animationPosition.y = Protect<float>(animationPositionTable[2]);
		animationPosition.x += _playerSpawn.x;
		animationPosition.y = _levelLength - animationPosition.y;

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
	sol::table spawnPoints = Protect<sol::table>(level["spawnPoints"]);
	for (int i = 1; i <= spawnPoints.size(); i++)
	{
		AirplaneSpawnPosition spawn;
		std::string id = Protect<std::string>(spawnPoints[i][1]);
		spawn.x = Protect<float>(spawnPoints[i][2]);
		spawn.y = Protect<float>(spawnPoints[i][3]);
		spawn.y = _levelLength - spawn.y;
		spawn.x += _playerSpawn.x;
		spawn.data = &_airplaneDataDict[id];
		_enemySpawns.push_back(spawn);
	}

	//Add texts
	sol::table texts = Protect<sol::table>(level["texts"]);
	for (int i = 1; i <= texts.size(); i++)
	{
		TextAnimationData tdata;
		sol::table text = Protect<sol::table>(texts[i]);
		tdata.str = _context->player->Parse(Protect<std::string>(text["text"]));
		tdata.y = Protect<float>(text["y"]);
		tdata.y = _levelLength - tdata.y;
		tdata.timePerLetter = Protect<float>(text["timePerLetter"]);
		tdata.timeBeforeDestroy = Protect<float>(text["timeBeforeDestroy"]);
		tdata.charSize = Protect<int>(text["charSize"]);
		tdata.font = Protect<std::string>(text["font"]);
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

	_soundtrack = Protect<std::string>(level["soundtrack"]);
	_menuSoundtrack = Protect<std::string>(level["menuSoundtrack"]);
	_context->music->openFromFile(_soundtrack);
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
	dt *= _timeScale;
	_soundQueue.Update();
	RemoveOffScreenObjects(dt);
	HandleCollisions(dt);
	CapPlayerPosition();
	if (_playerAirplane && _playerAirplane->IsDestroyed())
	{
		_playerAirplane = nullptr;
		_gameOver = true;
		std::shared_ptr<LoseState> lose(new LoseState(_context, this));
		if (!OnTop()) // LocalMenu is up
		{
			RequestPop();
		}
		RequestPush(lose);
		Command blockShooting;
		blockShooting.category = GameObject::EnemyAirplane | GameObject::PlayerAirplane;
		blockShooting.action = DeriveAction<Airplane>(
			[](Airplane& airplane, float dt)
			{
				airplane.SetBlockShooting(true);
			});
		_root->OnCommand(blockShooting, dt);
	}
	else if (_playerAirplane && !_win && _worldView.getCenter().y < _playerSpawn.y - _levelLength)
	{
		_gameOver = true;
		std::shared_ptr<WinState> win(new WinState(_context, this));
		if (!OnTop()) // LocalMenu is up
		{
			RequestPop();
		}
		RequestPush(win);
		_win = true;
		Command blockShooting;
		blockShooting.category = GameObject::EnemyAirplane | GameObject::PlayerAirplane;
		blockShooting.action = DeriveAction<Airplane>(
			[](Airplane& airplane, float dt)
			{
				airplane.SetBlockShooting(true);
			});
		_root->OnCommand(blockShooting, dt);
	}
	try
	{
		SpawnObjects();
		_context->player->HandleRealtimeInput(_commands);
		while (!_commands.IsEmpty())
		{
			_root->OnCommand(_commands.Pop(), dt);
		}
		_root->FixedUpdate(dt);
	}
	catch (sol::error& ex)
	{
		std::shared_ptr<MainMenu> mainMenu(new MainMenu(_context, false));
		std::shared_ptr<ErrorScene> error(new ErrorScene(_context, "Lua Error", ex.what(),
			std::move(mainMenu)));

		RequestPush(std::move(error));
	}

	if (_playerAirplane)
	{
		_playerAirplane->move(0, -_scrollSpeed * dt);
	}
	if (!_win)
	{
		//_worldView.move(0, -_scrollSpeed * dt);
		_actualViewPosition += -_scrollSpeed * dt;
		_worldView.setCenter(_playerSpawn.x, _actualViewPosition);
		_shaker.Update(dt);

	}
	if (_worldView.getCenter().y + _worldView.getSize().y / 2 < _background[_currentBgIndex]->getPosition().y - _background[_currentBgIndex]->GetBoundingRect().height / 2)
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

	dt *= _timeScale;
	try
	{
		_root->Update(dt);
	}
	catch (sol::error& ex)
	{
		std::shared_ptr<MainMenu> mainMenu(new MainMenu(_context, false));
		std::shared_ptr<ErrorScene> error(new ErrorScene(_context, "Lua Error", ex.what(), 
			std::move(mainMenu)));

		RequestPush(std::move(error));
	}
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
		if (ev.key.code == _context->player->GetKey(Player::Exit) && OnTop())
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
			std::cout << "NEW TEXT: " << _texts[0].y << ' ' << _worldView.getCenter().y << '\n';
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
			if (_gameOver)
			{
				airplanePtr->SetBlockShooting(true);
			}
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
	_background[_currentBgIndex]->setPosition(_context->window->getSize().x / 2, _background[_currentBgIndex]->getPosition().y - _background[_currentBgIndex]->GetBoundingRect().height * 2);

	if (_currentBgIndex == 0)
	{
		_currentBgIndex++;
	}
	else
	{
		_currentBgIndex = 0;
	}
}


void Level::PlaySound(const sf::SoundBuffer& buffer, float volume, float pitch)
{
	_soundQueue.PlaySound(buffer, volume, pitch);
}


void Level::ShakeScreen(float amplitude, float duration)
{
	_shaker.Shake(amplitude, duration);
}