#pragma once

/*
	At the time I created this class, I wasn't aware that it is actually named a Pause Menu.
	Can be pushed on top of the scene stack while playing the level to pause the game.
	The menu slows the game to a halt with a slow motion effect.
	The animation as a whole is achieved using a state machine.
*/

#include "Scene.h"
#include "GuiMenu.h"

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include "Utility/Animations.h"
#include "SettingsPanel.h"

class Level;

class LocalMenu :
	public Scene,
	public GuiMenu
{
	class LocalMenuState
	{
	protected:
		LocalMenu* _menu;

	public:
		LocalMenuState(LocalMenu* menu);

		virtual void Start() = 0;
		virtual void Draw() = 0;
		virtual void HandleEvent(const sf::Event& ev) = 0;
		virtual void Update(float dt) = 0;
	};

	class PushingState :
		public LocalMenuState
	{
		GenericFadeAnimation<sf::RectangleShape> _fadeInAnimation;
		float _elapsedTime;
		float _transitionDuration;

	public:
		PushingState(LocalMenu* menu);

		void Start() override;
		void Draw() override;
		void HandleEvent(const sf::Event& ev) override;
		void Update(float dt) override;
	};

	class IdleState :
		public LocalMenuState
	{
		tgui::Gui _gui;
		tgui::VerticalLayout::Ptr _panel;
		std::shared_ptr<SettingsPanel> _settingsPanel;
		tgui::Button::Ptr _resume;
		tgui::Button::Ptr _exit;
		tgui::Button::Ptr _settings;

	public:
		IdleState(LocalMenu* menu);

		void Start() override;
		void Draw() override;
		void HandleEvent(const sf::Event& ev) override;
		void Update(float dt) override;

		void SetVisible(bool visible);

		void LateInit();
	};

	class PoppingState : 
		public LocalMenuState
	{
		GenericFadeAnimation<sf::RectangleShape> _fadeOutAnimation;
		float _elapsedTime;
		float _transitionDuration;

	public:
		PoppingState(LocalMenu* menu);

		void Start() override;
		void Draw() override;
		void HandleEvent(const sf::Event& ev) override;
		void Update(float dt) override;
	};

	sf::RectangleShape _background;

	Level* _level;
	float _lowestPitch;
	int _highestAlpha;

	LocalMenuState* _currentState;
	PushingState _push;
	PoppingState _pop;
	IdleState _idle;

	sf::Music _music;
	
	sf::Sound _click;

	bool _initialized;

public:
	LocalMenu(Context* context);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool FixedUpdate(float dt) override;
	bool Render() override;
	void Enable() override;
	void Disable() override;

	void SetVisible(bool visible) override;

	void SetLevel(Level* lvl) { _level = lvl; }


private:
	void StartPushingState();
	void StartPoppingState();
	void StartIdleState();
};