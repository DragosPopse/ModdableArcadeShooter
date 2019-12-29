#include "Scenes/LevelLoader.h"

#include "Utility/Math.h"

#include "Scenes/ErrorScene.h"
#include "Scenes/MainMenu.h"


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

	_sprite.setPosition(static_cast<float>(_windowSize.x) + _sprite.getLocalBounds().width / 2.f, static_cast<float>(_windowSize.y) / 2.f);

	_thread.launch();
}


bool LevelLoader::Update(float dt)
{
	if (IsFinished())
	{
		if (!_error)
		{
			RequestClear();
			std::shared_ptr<Scene> ptr(_result);
			RequestPush(std::move(ptr));
		}
		else
		{
			std::shared_ptr<MainMenu> mainMenu(new MainMenu(_context, false));
			std::shared_ptr<ErrorScene> error(new ErrorScene(_context, "Lua Error", _errorMessage, std::move(mainMenu)));
			RequestPush(std::move(error));
		}
	}
	else
	{
		_sprite.move(-_speed * dt, 0);
		if (_sprite.getPosition().x + _sprite.getLocalBounds().width < 0)
		{
			_sprite.setPosition(static_cast<float>(_windowSize.x) + static_cast<float>(_sprite.getLocalBounds().width) / 2.f, static_cast<float>(_windowSize.y) / 2.f);
		}
	}

	

	return false;
}


bool LevelLoader::Render()
{
	_context->window->draw(_sprite);
	return true;
}


void LevelLoader::RunTask()
{
	
	//sf::Clock clock;
	//clock.restart();
	//while (clock.getElapsedTime().asSeconds() < 3.f)
	//{
	//	//Dummy loop just for testing multithreading 
	//}
	
	try
	{
		_result = new Level(_context, _fileName);
	}
	catch(sol::error& err)
	{
		_error = true;
		_errorMessage = err.what();
	}
	
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