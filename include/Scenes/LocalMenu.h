#pragma once


#include "Scene.h"
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "Utility.h"

class Level;

class LocalMenu :
	public Scene
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

	public:
		IdleState(LocalMenu* menu);

		void Start() override;
		void Draw() override;
		void HandleEvent(const sf::Event& ev) override;
		void Update(float dt) override;
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
	float _highestVolume;
	int _highestAlpha;

	LocalMenuState* _currentState;
	PushingState _push;
	PoppingState _pop;
	IdleState _idle;

	sf::Music _music;

public:
	LocalMenu(Context* context);

	bool HandleEvent(const sf::Event& ev) override;
	bool Update(float dt) override;
	bool FixedUpdate(float dt) override;
	bool Render() override;
	void SetLevel(Level* lvl) { _level = lvl; }

	void Start();

private:
	void StartPushingState();
	void StartPoppingState();
	void StartIdleState();
};