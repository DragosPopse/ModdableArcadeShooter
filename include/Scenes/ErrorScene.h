#pragma once


#include "Scene.h"

#include <string>

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

/*
	Some errors should not crash the game, so an error panel is displayed using this scene
	Even if for consistency sake it should implement GuiMenu interface, it doesn't require it's functionality.	
*/
class ErrorScene :
	public Scene
{
	tgui::Gui _gui;

	tgui::Panel::Ptr _panel;
	tgui::TextBox::Ptr _message;
	tgui::Label::Ptr _title;
	tgui::Button::Ptr _confirmButton;
	std::shared_ptr<Scene> _nextScene;

public:
	ErrorScene(Context* context, const std::string& title, const std::string& message, std::shared_ptr<Scene> nextScene = nullptr);

	bool HandleEvent(const sf::Event& ev);
	bool Render();
};