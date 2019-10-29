#include "Scenes/MainMenu.h"
#include <iostream>
#include <SFML/Audio.hpp>


MainMenu::MainMenu(Context* context) :
	Scene(context),
	_gui(*context->window),
	_scrollSpeed(60),
	_firstIndex(1),
	_nextIndex(0),
	_fade(0.9, 0),
	_elapsedTime(0),
	_fadeDuration(0.5f)
{
	_view = _context->window->getDefaultView();
	_textures.Load("Background", "assets/textures/MenuBackground.png");
	_parts[0].setTexture(_textures["Background"]);
	_parts[1].setTexture(_textures["Background"]);
	float scale = (float)_context->window->getSize().x / _textures["Background"].getSize().x;
	_parts[0].setScale(scale, scale);
	_parts[1].setScale(scale, scale);
	_parts[1].setPosition(0, _parts[0].getPosition().y - _parts[0].getGlobalBounds().height);
}


bool MainMenu::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);
	if (ev.type == sf::Event::Closed)
	{
		RequestClear();
	}
	return false;
}


bool MainMenu::Update(float dt)
{
	_elapsedTime += dt;
	if (_elapsedTime < _fadeDuration)
	{
		_fade(_parts[0], _elapsedTime / _fadeDuration);
		_fade(_parts[1], _elapsedTime / _fadeDuration);
	}
	_view.move(0, -_scrollSpeed * dt);
	if (_view.getCenter().y < _parts[_firstIndex].getPosition().y + _parts[_firstIndex].getGlobalBounds().height / 2)
	{
		
		SwitchSprites();
	}
	return false;
}


bool MainMenu::Render()
{
	_context->window->setView(_view);
	_context->window->draw(_parts[0]);
	_context->window->draw(_parts[1]);
	_context->window->setView(_context->window->getDefaultView());
	_gui.draw();
	
	return false;
}


void MainMenu::SwitchSprites()
{
	_parts[_nextIndex].setPosition(0, _parts[_firstIndex].getPosition().y - _parts[_firstIndex].getGlobalBounds().height);

	int aux = _firstIndex;
	_firstIndex = _nextIndex;
	_nextIndex = aux;
}