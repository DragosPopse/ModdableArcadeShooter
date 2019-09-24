#pragma once

#include "Component.h"

class SpriteComponent;


class MoveComponent :
	public Component
{
	SpriteComponent* _sprite;
	float _speed;

public:
	void Init() override;

	void Start() override;

	void FixedUpdate(float dt) override;

	void SetSpeed(float speed);
};