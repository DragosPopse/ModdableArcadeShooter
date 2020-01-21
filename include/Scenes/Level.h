#pragma once

/*
	This is the actual gameplay loop.	
	The class loads the level and plays it.
	The level acts as a huge container for all the resources and objects required.
*/

#include "Scene.h"

#include <deque>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <Thor/Animations.hpp>
#include <sol/sol.hpp>

#include "Context.h"
#include "Player.h"
#include "CommandQueue.h"
#include "SoundQueue.h"
#include "ViewShaker.h"
#include "LocalMenu.h"

#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Animation.h"
#include "GameObjects/ParticleSystemObject.h"
#include "GameObjects/TextAnimation.h"
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
	sol::optional<sf::Color> color;
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
	friend class WinState;
	friend class LoseState;
	friend class LocalMenu;

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
	float _levelLength;
	float _scale;
	float _borderSize;

	float _fpsUpdateInterval;
	float _lastFpsUpdate;

	TextAnimation* _currentText;

	std::deque<AirplaneSpawnPosition> _enemySpawns;
	std::deque<Animation*> _environmentSpawns;
	std::deque<TextAnimationData> _texts;

	sf::Shader _flashShader;

	sf::Shader _vignetteShader;
	float _vignetteElapsedTime;
	float _vignetteDuration;
	float _vignetteFadeInRatio;
	float _vignetteFadeOutRatio;
	float _vignetteMaxIntensity;
	float _vignetteCurrentIntensity;
	float _vignetteInnerRadius;
	float _vignetteOuterRadius;
	sf::Glsl::Vec4 _vignetteColor;
	bool _playingVignette;

	sf::RenderTexture _renderTexture;

	std::shared_ptr<LocalMenu> _localMenu;

	std::vector<SpriteObject*> _background;
	int _currentBgIndex;

	std::string _saveFile;
	int _highScore;
	
	std::map<std::string, int> _enemiesDowned;
	std::map<std::string, int> _airplaneScores;
	std::string _defaultFont;

	bool _win;

	SoundQueue _soundQueue;
	std::string _soundtrack;
	std::string _menuSoundtrack;

	float _timeScale;

	ViewShaker _shaker;
	float _actualViewPosition; //so that the shaker doesn't desync the view

	bool _gameOver = false;

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

	/*
		Used by the Airplane objects to animate the damage taken.
	*/
	sf::Shader* GetFlashShader() { return &_flashShader; }

	void DisplayText();

	GameObject* GetEnemyProjectilesRoot() { return _enemyProjectilesRoot; }
	GameObject* GetEnemyAirplanesRoot() { return _enemyAirplanesRoot; }
	Airplane* GetPlayerAirplane() { return _playerAirplane; }
	GameObject* GetPlayerProjectilesRoot() { return _playerProjectilesRoot; }
	
	sf::Texture& GetTexture(const std::string& id) { return _textures[id]; }

	/*
		It contains too many parameters to be readable. 
		It is only used by Lua to animate the pickups.
	*/
	void PlayVignetteAnimation(const sf::Glsl::Vec4& color, float inner, float outer, float intensity, float fadeInRatio, float fadeOutRatio, float duration);

	/*
		Used to keep track of destroyed airplanes for calculating the score
	*/
	void AddDownedEnemy(const std::string& name) { _enemiesDowned[name]++; }

	sf::Font& GetDefaultFont() { return _fonts[_defaultFont]; }

	std::string GetDefaultFontID() { return _defaultFont; }

	int GetHighScore() const { return _highScore; }

	sf::Shader* GetVignette() { return &_vignetteShader; }

	void PlaySound(const sf::SoundBuffer& buffer, float volume, float pitch = 1);

	void ShakeScreen(float amplitude, float duration);

	GameObject* GetRoot() { return _root.get(); }

	void PlaySoundFromLua(const sol::table& table);

private:
	/*
		GameObjects are only spawned in the world when the view gets closer to their spawn position. 
		This gives a performance boost since only required objects are updated.
	*/
	void SpawnObjects();

	/*
		Offscreen objects are no longer required so they can be deleted
		Different offscreen rules apply for different kinds of objects
		EnemyAirplanes and animations are offscreen when their Y is higher than view.y
		Projectiles are offscreen when they are far enough from the view bounds.
	*/
	void RemoveOffScreenObjects(float dt);

	/*
		Collision detection.
	*/
	void HandleCollisions(float dt);

	/*
		Keep the player in the view bounds
		Different results can be achieved depending if the function is called before or after reading the player input
	*/
	void CapPlayerPosition();

	/*
		Switch the 2 background sprites between each other so that they create a seemless impression
	*/
	void SwitchBackground();

	sf::FloatRect GetViewBounds() const;
};