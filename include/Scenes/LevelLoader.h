#pragma once

#include "Level.h"
#include "ResourceHolder.h"


class LevelLoader :
	public Scene
{
	Level* _result;
	sf::Thread _thread;
	sf::Mutex _mutex;
	bool _finished;
	std::string _errorMessage;
	bool _error = false;

	sf::Sprite _sprite;
	TextureHolder _textures;
	sf::Vector2u _windowSize;
	std::string _fileName;
	float _speed;

public:
	LevelLoader(Context* context, const std::string& fileName);

	
	bool Update(float dt) override;
	bool Render() override;

private:
	void RunTask();
	bool IsFinished();
};