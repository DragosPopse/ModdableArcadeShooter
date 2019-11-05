#pragma once

#include "Scene.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MainMenu;

struct LevelData
{
	std::string file;
	std::string title;
	std::string previewImage;

	LevelData(const std::string& f, const std::string& t, const std::string& p) :
		file(f),
		title(t),
		previewImage(p)
	{
	}
};

class LevelSelector :
	public Scene
{
	MainMenu* _menu;
	sf::RectangleShape _background;

	std::vector<LevelData> _levelData;
	
	int _currentIndex;
	sf::Sprite _levelImage;
	sf::Text _levelTitle;
	sf::Text _infoText;
	
	
public:
	LevelSelector(Context* context, MainMenu* menu);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;

private:
	void UpdateDisplay();
};