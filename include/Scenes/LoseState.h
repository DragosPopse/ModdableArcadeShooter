#pragma once

#include "Scene.h"

#include <memory>

#include <TGUI/TGUI.hpp>
#include <Thor/Animations.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameObjects/NumberIncrementAnimation.h"
#include "GameObjects/TextObject.h"
#include "GameObjects/TextAnimation.h"

#include "Utility.h"

class Level;

class LoseState :
	public Scene
{
	Level* _level;

	sf::RectangleShape _background;
	GenericFadeAnimation<sf::RectangleShape> _backgroundAnimation;
	GenericFadeAnimation<TextObject> _textAnimation;
	GenericFadeAnimation<TextObject> _flickerAnimation;
	TextCharScaleAnimation<TextObject> _highScoreAnimation;

	float _elapsedTime;
	float _flickerElapsedTime;

	bool _backgroundFading;
	float _animationDuration;
	float _flickerDuration;
	bool _animateHighScore;

	float _letterDuration;

	float _scaleDuration;
	float _incrementDuration;
	bool _upscaling;
	size_t _charSize;
	size_t _finalCharSize;

	std::unique_ptr<TextObject> _yourScore;
	NumberIncrementAnimation* _score;
	
	std::unique_ptr<TextObject> _infoText;
	std::unique_ptr<TextAnimation> _summaryText;

	std::unique_ptr<TextObject> _highScoreText;
	TextObject* _highScore;

	int _currentIncrement;

	std::vector<std::string> _summaries;

public:
	LoseState(Context* context, Level* level);

	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& ev) override;
	bool Render() override;
	bool FixedUpdate(float dt) override;
};