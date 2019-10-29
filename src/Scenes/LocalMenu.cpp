#include "Scenes/LocalMenu.h"
#include <iostream>
#include "Scenes/Level.h"


LocalMenu::LocalMenu(Context* context) :
	Scene(context),
	_gui(*context->window)
{
	_background.setFillColor(sf::Color(0, 0, 0, 100));
	_background.setPosition(0, 0);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));

	tgui::Button::Ptr resume = tgui::Button::create("Resume");
	tgui::Button::Ptr exit = tgui::Button::create("Exit");

	
	resume->connect("pressed", 
		[this]()
		{
			RequestPop();
		});

	exit->connect("pressed", 
		[this]()
		{
			RequestClear();
		});

	resume->setSize("40%", "20%");
	resume->setTextSize(30);
	exit->setTextSize(30);
	exit->setSize("40%", "20%");
	tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
	layout->setPosition("(&.size - size) / 2", "&.size - size");
	layout->add(resume);
	layout->add(exit);
	layout->setSize("30%");
	
	
	_gui.add(layout);

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