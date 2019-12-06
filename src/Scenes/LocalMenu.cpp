#include "Scenes/LocalMenu.h"
#include <iostream>
#include "Scenes/Level.h"
#include "Scenes/MainMenu.h"


LocalMenu::LocalMenu(Context* context) :
	Scene(context),
	_lowestPitch(0.f),
	_currentState(nullptr),
	_push(this),
	_pop(this),
	_idle(this),
	_highestAlpha(100)
{
	_background.setPosition(0.f, 0.f);
	_background.setSize(sf::Vector2f(_context->window->getSize().x, _context->window->getSize().y));

	_fonts.Load("Menu", "assets/fonts/pcsenior.ttf");
	_sounds.Load("Click", "assets/audio/sfx/Click.wav");
	_click.setBuffer(_sounds["Click"]);

	_idle.LateInit();
}


bool LocalMenu::HandleEvent(const sf::Event& ev)
{
	_currentState->HandleEvent(ev);
	if (_currentState != &_idle)
	{
		return true;
	}
	return false;
}


bool LocalMenu::Update(float dt)
{
	//_currentState->Update(dt);
	if (_currentState != &_idle)
	{
		return true;
	}
	return false;
}


bool LocalMenu::FixedUpdate(float dt)
{
	_currentState->Update(dt);
	if (_currentState != &_idle)
	{
		return true;
	}
	return false;
}


bool LocalMenu::Render()
{
	_currentState->Draw();
	return true;
}


void LocalMenu::Enable()
{
	_music.openFromFile(_level->_menuSoundtrack);
	_music.setVolume(0.f);
	_music.setPitch(_lowestPitch);
	_music.play();
	SetVisible(true);
	StartPushingState();
}


void LocalMenu::Disable()
{
	_level->_timeScale = 1.f;
	_music.stop();
	_context->music->setVolume(_context->player->GetMusicVolume());
	_context->music->setPitch(1.f);
	for (auto& sound : _level->_soundQueue)
	{
		sound.setVolume(_context->player->GetSfxVolume());
		sound.setPitch(1.f);
	}
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


void LocalMenu::IdleState::LateInit()
{
	_settingsPanel.reset(new SettingsPanel(_menu->_context, static_cast<Menu*>(_menu), tgui::Font(_menu->_fonts["Menu"]), _menu->_sounds["Click"]));
	_resume->setInheritedFont(_menu->_fonts["Menu"]);
	_exit->setInheritedFont(_menu->_fonts["Menu"]);
	_settings->setInheritedFont(_menu->_fonts["Menu"]);
}


LocalMenu::IdleState::IdleState(LocalMenu* menu) :
	LocalMenuState(menu),
	_gui(*menu->_context->window),
	_settingsPanel()
{
	_resume = tgui::Button::create("Resume");
	_exit = tgui::Button::create("Exit");
	_settings = tgui::Button::create("Settings");


	_resume->connect("pressed",
		[this]()
		{
			_menu->_click.play();
			_menu->StartPoppingState();
		});

	_exit->connect("pressed",
		[this]()
		{
			_menu->_click.play();
			_menu->RequestClear();
			_menu->_context->music->setVolume(_menu->GetContext()->player->GetMusicVolume());
			_menu->_context->music->setPitch(1.f);
			std::shared_ptr<MainMenu> menu(new MainMenu(_menu->_context, false));
			_menu->RequestPush(menu);
		});

	_settings->connect("pressed",
		[this]()
		{
			_menu->_click.play();
			_menu->SetVisible(false);
			_settingsPanel->SetVisible(true);
			_menu->RequestPush(_settingsPanel);
		});

	_resume->setSize("40%", "20%");
	_resume->setTextSize(30);
	_exit->setTextSize(30);
	_settings->setTextSize(30);
	_exit->setSize("40%", "20%");
	_panel = tgui::VerticalLayout::create();
	_panel->setPosition("(&.size - size) / 2", "&.size - size");
	_panel->add(_resume);
	_panel->add(_settings);
	_panel->add(_exit);
	_panel->setSize("30%");


	_gui.add(_panel);
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


void LocalMenu::SetVisible(bool visible)
{
	_idle.SetVisible(visible);
}


void LocalMenu::IdleState::SetVisible(bool visible)
{
	_panel->setVisible(visible);
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
		float menuMusicPitch = Lerp(_menu->_lowestPitch, 1.f, progress);
		float menuMusicVolume = _menu->_context->player->GetMusicVolume();
		float levelMusicVolume = _menu->_context->player->GetMusicVolume();
		float levelSfxVolume = _menu->_context->player->GetSfxVolume();
		//float levelMusicVolume = Lerp(_menu->_highestVolume, 0.f, progress);
		//float menuMusicVolume = Lerp(0.f, _menu->_highestVolume, progress);
		_menu->_level->_timeScale = Lerp(1.f, 0.f, progress);

		_menu->_context->music->setVolume(levelMusicVolume);
		_menu->_context->music->setPitch(levelMusicPitch);
		_menu->_music.setVolume(menuMusicVolume);
		_menu->_music.setPitch(menuMusicPitch);

		for (auto& sound : _menu->_level->_soundQueue)
		{
			sound.setVolume(levelSfxVolume);
			sound.setPitch(levelMusicPitch);
		}
	}
	else
	{
		_menu->_level->_timeScale = 0.f;
		_menu->_context->music->pause();
		_menu->_music.setVolume(_menu->GetContext()->player->GetMusicVolume());
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
	_menu->_music.setVolume(_menu->_context->player->GetMusicVolume()); //in case volume changed in settings
}


void LocalMenu::PoppingState::Update(float dt)
{
	_elapsedTime += dt;
	float progress = _elapsedTime / _transitionDuration;
	if (progress < 1.f)
	{
		_fadeOutAnimation(_menu->_background, progress);
		float menuMusicPitch = Lerp(1.f, _menu->_lowestPitch, progress);
		float levelMusicPitch = Lerp(_menu->_lowestPitch, 1.f, progress);
		//float menuMusicVolume = Lerp(_menu->_highestVolume, 0.f, progress);
		//float levelMusicVolume = Lerp(0.f, _menu->_highestVolume, progress);
		float menuMusicVolume = _menu->GetContext()->player->GetMusicVolume();
		float levelMusicVolume = _menu->GetContext()->player->GetMusicVolume();
		float levelSfxVolume = _menu->GetContext()->player->GetSfxVolume();
		_menu->_level->_timeScale = Lerp(0.f, 1.f, progress);
		_menu->_context->music->setVolume(levelMusicVolume);
		_menu->_context->music->setPitch(levelMusicPitch);
		_menu->_music.setVolume(menuMusicVolume);
		_menu->_music.setPitch(menuMusicPitch);
		for (auto& sound : _menu->_level->_soundQueue)
		{
			sound.setVolume(levelSfxVolume);
			sound.setPitch(levelMusicPitch);
		}
	}
	else
	{
		_menu->RequestPop();
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
