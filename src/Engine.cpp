#include "Engine.h"
#include "LuaInit.h"
#include "Context.h"


Engine::Engine() : 
	_fixedDeltaTime(1.f / 60.f),
	_deltaTime(0.f)
{
	LuaInit_Base(_lua);
}


void Engine::Run()
{
	_clock.restart();
	while (_window.isOpen())
	{
		_deltaTime = _clock.restart().asSeconds();
		float timeSinceLastFrame = _deltaTime;
		HandleEvents();
		Update();
		while (timeSinceLastFrame > _fixedDeltaTime)
		{
			timeSinceLastFrame -= _fixedDeltaTime;
			FixedUpdate();
		}
	}
}


void Engine::HandleEvents()
{
	
}


void Engine::Update()
{

}

void Engine::FixedUpdate()
{

}

void Engine::Render()
{

}