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
	bool good;

	LevelData(const std::string& f, const std::string& t, const std::string& p, int hs, bool g) :
		file(f),
		title(t),
		previewImage(p),
		highScore(hs),
		good(g)
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

	sf::Keyboard::Key _fireKey;
	sf::Keyboard::Key _leftKey;
	sf::Keyboard::Key _rightKey;
	sf::Keyboard::Key _exitKey;
	
	
public:
	LevelSelector(Context* context, MainMenu* menu);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool Render() override;
	void Enable() override;

private:
	void UpdateDisplay();
};