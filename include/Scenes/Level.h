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
#include <sol/sol.hpp>
#include <GameObjects/TextAnimation.h>
#include <Thor/Animations.hpp>
#include <memory>
#include "LocalMenu.h"
#include "GameObjects/Pickup.h"

class Airplane;
class Projectile;

struct TextAnimationData
{
	float y;
	float timePerLetter;
	float timeBeforeDestroy;
	unsigned int charSize;
	std::string str;
	std::string font;
};

struct AirplaneSpawnPosition
{
	AirplaneData* data;
	float x;
	float y;
};

struct PickupSpawnPosition //WIP
{
	PickupData* data;
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
	GameObject* _pickupsRoot;
	std::unique_ptr<GameObject> _uiRoot;
	Airplane* _playerAirplane;

	std::map<std::string, AirplaneData> _airplaneDataDict;
	std::map<std::string, ProjectileData> _projectileDataDict;
	std::map<std::string, PickupData> _pickupDataDict;

	CommandQueue _commands;

	sf::View _worldView;
	sf::FloatRect _worldBounds;
	sf::Vector2f _playerSpawn;
	float _scrollSpeed;
	float _worldHeight;
	float _scale;

	float _fpsUpdateInterval;
	float _lastFpsUpdate;

	TextAnimation* _currentText;

	std::deque<AirplaneSpawnPosition> _enemySpawns;
	std::deque<Animation*> _environmentSpawns;
	std::deque<TextAnimationData> _texts;

	sf::Shader _flashShader;

	std::shared_ptr<LocalMenu> _localMenu;

public:
	Level(Context* context, const std::string& fileName);

	~Level();

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
	void AddLuaParticle(sol::userdata p);
	void AddPickup(Pickup* pickup);

	const sf::Font& GetFont(const std::string& id) { return _fonts[id]; }

	sf::Shader* GetFlashShader() { return &_flashShader; }

	void DisplayText();

	GameObject* GetEnemyProjectilesRoot() { return _enemyProjectilesRoot; }
	GameObject* GetEnemyAirplanesRoot() { return _enemyAirplanesRoot; }
	Airplane* GetPlayerAirplane() { return _playerAirplane; }
	GameObject* GetPlayerProjectilesRoot() { return _playerProjectilesRoot; }
	
	sf::Texture& GetTexture(const std::string& id) { return _textures[id]; }

private:
	void SpawnObjects();
	void RemoveOffScreenObjects(float dt);

	void HandleCollisions(float dt);
};