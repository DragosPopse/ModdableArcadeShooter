#pragma once

/*
	Scene pushed when completing the level. Same functionality as LoseState but with a vignette animation instead of fade-to-black
*/

#include "Scene.h"

#include <memory>

#include <TGUI/TGUI.hpp>
#include <Thor/Animations.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameObjects/NumberIncrementAnimation.h"
#include "GameObjects/TextObject.h"
#include "GameObjects/TextAnimation.h"

#include "Utility/Animations.h"


class Level;

class WinState :
	public Scene
{
	Level* _level;

	sf::RectangleShape _background;
	GenericFadeAnimation<sf::RectangleShape> _backgroundAnimation;
	GenericFadeAnimation<TextObject> _textAnimation;
	GenericFadeAnimation<TextObject> _flickerAnimation;
	TextCharScaleAnimation<TextObject> _highScoreAnimation;

	float _elapsedTime;
	float _fixedElapsedTime;
	float _flickerElapsedTime;

	bool _backgroundFading;
	float _animationDuration;
	float _flickerDuration;
	bool _animateHighScore;

	float _letterDuration;

	float _scaleDuration;
	float _incrementDuration;
	bool _upscaling;
	unsigned int _charSize;
	unsigned int _finalCharSize;

	std::unique_ptr<TextObject> _yourScore;
	NumberIncrementAnimation* _score;

	std::unique_ptr<TextObject> _infoText;
	std::unique_ptr<TextAnimation> _summaryText;

	std::unique_ptr<TextObject> _highScoreText;
	TextObject* _highScore;

	int _currentIncrement;

	std::vector<std::string> _summaries;
	sf::Shader* _vignette;
	sf::Glsl::Vec4 _vignetteColor;
	float _vignetteInner;
	float _currentVignetteInner;
	float _currentVignetteOuter;
	float _vignetteOuter;
	float _zoomFactor;
	float _vignetteDuration;
	float _zoomDuration;
	float _maxDuration;
	float _vignetteIntensity;
	float _currentZoomFactor;
	sf::Vector2f _startViewCenter;
	

public:
	WinState(Context* context, Level* level);

	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& ev) override;
	bool Render() override;
	bool FixedUpdate(float dt) override;

private:
	void CalculateVignetteParams();
	void CapPlayerHorizontalPosition(); //This overrides the one in Level because the view now follows the player
};