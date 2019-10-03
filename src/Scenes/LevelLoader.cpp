#include "Scenes/LevelLoader.h"
#include "Utility.h"


LevelLoader::LevelLoader(Context* context, const std::string& fileName) :
	Scene(context),
	_result(nullptr),
	_finished(false),
	_thread(&LevelLoader::RunTask, this),
	_fileName(fileName),
	_speed(700.f)
{
	_textures.Load("Load", "assets/textures/Loading.png");
	_sprite.setTexture(_textures["Load"]);
	
	CenterOrigin(_sprite);
	
	_windowSize = _context->window->getSize();

	_sprite.setPosition(_windowSize.x + _sprite.getLocalBounds().width / 2, _windowSize.y / 2);

	_thread.launch();
}


bool LevelLoader::Update(float dt)
{
	if (IsFinished())
	{
		RequestClear();
		RequestPush(_result);
	}
	else
	{
		_sprite.move(-_speed * dt, 0);
		if (_sprite.getPosition().x + _sprite.getLocalBounds().width < 0)
		{
			_sprite.setPosition(_windowSize.x + _sprite.getLocalBounds().width / 2, _windowSize.y / 2);
		}
	}

	

	return false;
}


bool LevelLoader::Render()
{
	_context->window->draw(_sprite);
	return false;
}


void LevelLoader::RunTask()
{
	_result = new Level(_context, _fileName);
	

	{
		sf::Lock lock(_mutex);
		_finished = true;
	}
}


bool LevelLoader::IsFinished()
{
	sf::Lock lock(_mutex);
	return _finished;
}