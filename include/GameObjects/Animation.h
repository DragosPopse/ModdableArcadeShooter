#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>


class Animation :
	public GameObject
{
	sf::Sprite _sprite;
	float _elapsedTime;
	float _timePerFrame;
	int _numberOfFrames;
	bool _finished;
	bool _destroyOnFinish;
	sf::IntRect _currentFrame;
	sf::IntRect _firstFrame;
	int _currentIndex;
	bool _loop;

public:
	Animation();

	void Update(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetTexture(const sf::Texture& texture);
	void SetFirstRect(const sf::IntRect& rect);
	
	void SetTimePerFrame(float time);
	void SetNumberOfFrames(int n);
	void SetDestroyOnFinish(bool b);
	bool IsFinished() const;
	void SetLoopable(bool b);

	void SetIndex(int n);

	sf::FloatRect GetBoundingRect() const override { return GetWorldTransform().transformRect(_sprite.getGlobalBounds()); }
};