#pragma once

#include "Scene.h"
#include <TGUI/TGUI.hpp>
#include <Thor/Animations.hpp>
#include "Utility.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObjects/NumberIncrementAnimation.h"
#include "GameObjects/TextObject.h"
#include <memory>

class Level;


class LoseState :
	public Scene
{
	Level* _level;

	sf::RectangleShape _background;
	GenericFadeAnimation<sf::RectangleShape> _backgroundAnimation;
	GenericFadeAnimation<TextObject> _textAnimation;
	TextCharScaleAnimation<TextObject> _highScoreAnimation;

	float _elapsedTime;

	bool _backgroundFading;
	float _animationDuration;
	bool _animateHighScore;

	float _scaleDuration;
	float _incrementDuration;
	bool _upscaling;
	size_t _charSize;
	size_t _finalCharSize;

	std::unique_ptr<TextObject> _yourScore;
	NumberIncrementAnimation* _score;

	std::unique_ptr<TextObject> _highScoreText;
	TextObject* _highScore;

public:
	LoseState(Context* context, Level* level);

	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& ev) override;
	bool Render() override;
	bool FixedUpdate(float dt) override;
};