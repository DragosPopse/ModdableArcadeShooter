#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include "GameObjects/Animation.h"

class Airplane;
class Level;

struct PickupData
{
	sol::function onPickup;
	std::string texture;
	sf::IntRect firstRect;
	int frames;
	float frameDuration;
	float scale; //WIP

	std::string destroyTexture;
	sf::IntRect destroyFirstRect;
	int destroyFrames;
	float destroyFrameDuration;
	float destroyScale;
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

	unsigned int GetCategory() const override { return GameObject::PickupItem; }

	sf::FloatRect GetBoundingRect() const override { return _animation->GetBoundingRect(); }

	bool WasUsed() const { return _playingDestroyAnimation; }
};