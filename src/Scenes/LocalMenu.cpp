#include "Scenes/LocalMenu.h"
#include <iostream>
#include "Scenes/Level.h"
#include "Scenes/MainMenu.h"


LocalMenu::LocalMenu(Context* context) :
	Scene(context),
	_lowestPitch(0.5),
	_currentState(nullptr),
	_push(this),
	_pop(this),
	_idle(this),
	_highestAlpha(100)
{
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));

	

}


bool LocalMenu::HandleEvent(const sf::Event& ev)
{
	_currentState->HandleEvent(ev);
	return false;
}


bool LocalMenu::Update(float dt)
{
	//_currentState->Update(dt);
	return true;
}


bool LocalMenu::FixedUpdate(float dt)
{
	_currentState->Update(dt);
	return true;
}


bool LocalMenu::Render()
{
	_currentState->Draw();
	return true;
}


void LocalMenu::Start()
{
	_music.openFromFile(_level->_menuSoundtrack);
	_music.setVolume(0.f);
	_music.setPitch(_lowestPitch);
	_music.play();
	_highestVolume = _context->player->GetMusicVolume();
	StartPushingState();
}


void LocalMenu::StartPushingState()
{
	_push.Start();
	_currentState = &_push;
}


void LocalMenu::StartIdleState()
{
	_idle.Start();
	_currentState = &_idle;
}


void LocalMenu::StartPoppingState()
{
	_pop.Start();
	_currentState = &_pop;
}


LocalMenu::LocalMenuState::LocalMenuState(LocalMenu* menu) :
	_menu(menu)
{
}


LocalMenu::PushingState::PushingState(LocalMenu* menu) :
	LocalMenuState(menu),
	_fadeInAnimation(1.f, 0.f,
		[](sf::RectangleShape& animated, const sf::Color& color)
		{
			animated.setFillColor(color);
		},
		[](const sf::RectangleShape& animated)
		{
			return animated.getFillColor();
		}),
	_transitionDuration(1.f)
{
	_fadeInAnimation.SetHighestAlpha(_menu->_highestAlpha);
}


LocalMenu::IdleState::IdleState(LocalMenu* menu) :
	LocalMenuState(menu),
	_gui(*menu->_context->window)
{
	tgui::Button::Ptr resume = tgui::Button::create("Resume");
	tgui::Button::Ptr exit = tgui::Button::create("Exit");


	resume->connect("pressed",
		[this]()
		{
			_menu->StartPoppingState();
		});

	exit->connect("pressed",
		[this]()
		{
			_menu->RequestClear();
			_menu->_context->music->setVolume(_menu->_highestVolume);
			_menu->_context->music->setPitch(1.f);
			std::shared_ptr<MainMenu> menu(new MainMenu(_menu->_context, false));
			_menu->RequestPush(menu);
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


LocalMenu::PoppingState::PoppingState(LocalMenu* menu) :
	LocalMenuState(menu),
	_fadeOutAnimation(0.f, 1.f,
		[](sf::RectangleShape& animated, const sf::Color& color)
		{
			animated.setFillColor(color);
		},
		[](const sf::RectangleShape& animated)
		{
			return animated.getFillColor();
		}),
	_transitionDuration(3.f)	
{
	_fadeOutAnimation.SetHighestAlpha(_menu->_highestAlpha);
}


void LocalMenu::PushingState::Start()
{
	_menu->_background.setFillColor(sf::Color(0, 0, 0, 0));
	_elapsedTime = 0.f;
}


void LocalMenu::IdleState::Start()
{
	_menu->_background.setFillColor(sf::Color(0, 0, 0, _menu->_highestAlpha));
}


void LocalMenu::PoppingState::Start()
{
	_elapsedTime = 0.f;
	_menu->_context->music->setVolume(0.f);
	_menu->_context->music->setPitch(_menu->_lowestPitch);
	_menu->_context->music->play();
	
	for (auto& sound : _menu->_level->_soundQueue)
	{
		if (sound.getStatus() != sf::Sound::Status::Stopped)
		{
			sound.play();
		} 
	}
}


void LocalMenu::PushingState::Update(float dt)
{
	_elapsedTime += dt;
	float progress = _elapsedTime / _transitionDuration;
	if (progress <= 1.f)
	{
		_fadeInAnimation(_menu->_background, progress);
		float levelMusicPitch = Lerp(1.f, _menu->_lowestPitch, progress);
		float levelMusicVolume = Lerp(_menu->_highestVolume, 0.f, progress);
		float menuMusicPitch = Lerp(_menu->_lowestPitch, 1.f, progress);
		float menuMusicVolume = Lerp(0.f, _menu->_highestVolume, progress);
		_menu->_level->_timeScale = Lerp(1.f, 0.f, progress);

		_menu->_context->music->setVolume(levelMusicVolume);
		_menu->_context->music->setPitch(levelMusicPitch);
		_menu->_music.setVolume(menuMusicVolume);
		_menu->_music.setPitch(menuMusicPitch);

		for (auto& sound : _menu->_level->_soundQueue)
		{
			sound.setVolume(levelMusicVolume);
			sound.setPitch(levelMusicPitch);
		}
	}
	else
	{
		_menu->_level->_timeScale = 0.f;
		_menu->_context->music->pause();
		_menu->_music.setVolume(_menu->_highestVolume);
		_menu->_music.setPitch(1.f);
		_menu->StartIdleState();
		for (auto& sound : _menu->_level->_soundQueue)
		{
			if (sound.getStatus() != sf::Sound::Status::Stopped)
			{
				sound.pause();
			}
		}
	}
	
}


void LocalMenu::IdleState::Update(float dt)
{

}


void LocalMenu::PoppingState::Update(float dt)
{
	_elapsedTime += dt;
	float progress = _elapsedTime / _transitionDuration;
	if (progress < 1.f)
	{
		_fadeOutAnimation(_menu->_background, progress);
		float menuMusicPitch = Lerp(1.f, _menu->_lowestPitch, progress);
		float menuMusicVolume = Lerp(_menu->_highestVolume, 0.f, progress);
		float levelMusicPitch = Lerp(_menu->_lowestPitch, 1.f, progress);
		float levelMusicVolume = Lerp(0.f, _menu->_highestVolume, progress);
		_menu->_level->_timeScale = Lerp(0.f, 1.f, progress);
		_menu->_context->music->setVolume(levelMusicVolume);
		_menu->_context->music->setPitch(levelMusicPitch);
		_menu->_music.setVolume(menuMusicVolume);
		_menu->_music.setPitch(menuMusicPitch);
		for (auto& sound : _menu->_level->_soundQueue)
		{
			sound.setVolume(levelMusicVolume);
			sound.setPitch(levelMusicPitch);
		}
	}
	else
	{
		_menu->_level->_timeScale = 1.f;
		_menu->_music.stop();
		_menu->_context->music->setVolume(_menu->_highestVolume);
		_menu->_context->music->setPitch(1.f);
		_menu->RequestPop();
		for (auto& sound : _menu->_level->_soundQueue)
		{
			sound.setVolume(_menu->_highestVolume);
			sound.setPitch(1.f);
		}
	}
}


void LocalMenu::PushingState::Draw() 
{
	_menu->_context->window->setView(_menu->_context->window->getDefaultView());
	_menu->_context->window->draw(_menu->_background);
}


void LocalMenu::IdleState::Draw() 
{
	_menu->_context->window->setView(_menu->_context->window->getDefaultView());
	_menu->_context->window->draw(_menu->_background);
	_gui.draw();

}


void LocalMenu::PoppingState::Draw() 
{
	_menu->_context->window->setView(_menu->_context->window->getDefaultView());
	_menu->_context->window->draw(_menu->_background);
}


void LocalMenu::PushingState::HandleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		_menu->RequestClear();
		break;
	}
}


void LocalMenu::IdleState::HandleEvent(const sf::Event& ev)
{
	_gui.handleEvent(ev);
	switch (ev.type)
	{
	case sf::Event::Closed:
		_menu->RequestClear();
		break;
	case sf::Event::KeyPressed:
		if (ev.key.code == sf::Keyboard::Escape)
		{
			_menu->StartPoppingState();
		}
	}
}


void LocalMenu::PoppingState::HandleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		_menu->RequestClear();
		break;
	}
}


