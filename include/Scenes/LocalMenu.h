#pragma once


#include "Scene.h"
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>


class LocalMenu :
	public Scene
{
	tgui::Gui _gui;
	sf::RectangleShape _background;

public:
	LocalMenu(Context* context);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;

};