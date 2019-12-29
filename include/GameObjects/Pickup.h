#pragma once

#include "GameObject.h"

#include <random>

#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>

#include "GameObjects/Animation.h"
#include "RandomizedSound.h"

class Airplane;
class Level;
class Animation;

struct PickupData
{
	sol::function onPickup;
	sf::Texture* texture = nullptr;
	sf::IntRect firstRect;
	int frames = 0;
	float frameDuration = 0;
	float scale = 0; //WIP

	sf::Texture* destroyTexture = nullptr;
	sf::IntRect destroyFirstRect;
	int destroyFrames = 0;
	float destroyFrameDuration = 0;
	float destroyScale = 0;

	std::mt19937 rng;
	RandomizedSound onPickupSound;
};


class Pickup :
	public GameObject
{
	PickupData* _data;
	Level* _level;
	Animation* _animation;

	bool _playingDestroyAnimation;

public:
	Pickup(PickupData* data);

	void Start(Scene* scene) override;

	void Update(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void OnPickup(Airplane& player);

	unsigned int GetCategory() const override { return static_cast<unsigned int>(Type::Pickup); }

	sf::FloatRect GetBoundingRect() const override { return _animation->GetBoundingRect(); }

	bool WasUsed() const { return _playingDestroyAnimation; }
};