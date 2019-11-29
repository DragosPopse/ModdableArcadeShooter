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
	int highScore;

	LevelData(const std::string& f, const std::string& t, const std::string& p, int hs) :
		file(f),
		title(t),
		previewImage(p),
		highScore(hs)
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
	sf::Text _levelScore;
	sf::Text _infoText;
	
	
public:
	LevelSelector(Context* context, MainMenu* menu);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;
	void Enable() override;

private:
	void UpdateDisplay();
};