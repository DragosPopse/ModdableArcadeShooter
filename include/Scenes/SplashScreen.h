#pragma once

#include "Scene.h"

#include <memory>

#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>

#include "Scenes/MainMenu.h"

class SplashScreen :
	public Scene
{
	sf::Sprite _splash;
	thor::FadeAnimation _animation;
	float _elapsedTime;
	std::shared_ptr<MainMenu> _mainMenu;

public:
	SplashScreen(Context* context);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;
};