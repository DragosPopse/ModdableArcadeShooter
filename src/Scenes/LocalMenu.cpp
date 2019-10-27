#include "Scenes/LocalMenu.h"
#include <iostream>


LocalMenu::LocalMenu(Context* context) :
	Scene(context),
	_gui(*context->window)
{
	_background.setFillColor(sf::Color(0, 0, 0, 100));
	_background.setPosition(0, 0);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));
	std::cout << "BG: " << _background.getSize().x << ' ' << _background.getSize().y << '\n';
}


bool LocalMenu::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
	{
		RequestPop();
	}
	return false;
}


bool LocalMenu::Update(float dt)
{
	return false;
}


bool LocalMenu::Render()
{
	_context->window->setView(_context->window->getDefaultView());
	_context->window->draw(_background);
	_gui.draw();
	return true;
}