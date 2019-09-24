#include <iostream>

#include "Components/MoveComponent.h"
#include "Components/SpriteComponent.h"
#include "GameObject.h"


void MoveComponent::Init()
{
	_speed = 0.f;
}


void MoveComponent::Start()
{
	_sprite = &_gameObject->GetComponent<SpriteComponent>();
}


void MoveComponent::FixedUpdate(float dt)
{
	_sprite->GetSprite().move(_speed * dt, _speed * dt);
}


void MoveComponent::SetSpeed(float speed)
{
	_speed = speed;
}