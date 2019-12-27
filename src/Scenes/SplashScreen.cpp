#include "Scenes/SplashScreen.h"

#include <SFML/Audio.hpp>
#include "Utility/Math.h"


SplashScreen::SplashScreen(Context* context) :
	Scene(context),
	_animation(0.30, 0.50),
	_elapsedTime(0),
	_mainMenu(new MainMenu(context, true))
{
	_textures.Load("Logo", "assets/textures/Logo.png");
	_splash.setTexture(_textures["Logo"]);
	_splash.setScale(sf::Vector2f(0.25, 0.25));
	CenterOrigin(_splash);
	_splash.setPosition(_context->window->getSize().x / 2, _context->window->getSize().y / 2);
	_context->music->openFromFile("assets/audio/music/MainMenu.wav");
	_context->music->setLoop(true);
	_context->music->setVolume(_context->player->GetMusicVolume());
	_context->music->play();
}


bool SplashScreen::HandleEvent(const sf::Event& ev)
{
	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}
	return false;
}


bool SplashScreen::Update(float dt)
{
	_elapsedTime += dt;
	if (_elapsedTime < 8.f)
	{
		_animation(_splash, _elapsedTime / 8.f);
	}
	else
	{
		RequestPop();
		RequestPush(_mainMenu);
	}
	
	return false;
}


bool SplashScreen::Render()
{
	_context->window->draw(_splash);
	return false;
}