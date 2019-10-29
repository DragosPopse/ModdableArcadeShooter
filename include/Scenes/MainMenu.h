#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <Thor/Animations.hpp>


class MainMenu :
	public Scene
{
	tgui::Gui _gui;
	sf::View _view;
	float _scrollSpeed;
	sf::Sprite _parts[2];

	int _firstIndex;
	int _nextIndex;

	thor::FadeAnimation _fade;
	float _elapsedTime;
	float _fadeDuration;

public:
	MainMenu(Context* context);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;

private:
	void SwitchSprites();
};