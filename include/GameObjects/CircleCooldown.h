#pragma once

/*
	Animation for the cooldown display
*/

#include "GameObject.h"

#include <SFML/Graphics.hpp>


class CircleCooldown :
	public GameObject	
{
	sf::Vector2f _topLeft;
	sf::Vector2f _topRight;
	sf::Vector2f _bottomLeft;
	sf::Vector2f _bottomRight;
	sf::Vector2f _center;
	sf::VertexArray _vertices;

	float _progress;
	float _elapsedTime;
	float _duration;
	bool _animationRunning;

	sf::Color _color;

public:
	CircleCooldown();

	void SetVertices(sf::Vector2f topLeft,
		sf::Vector2f topRight,
		sf::Vector2f bottomRight,
		sf::Vector2f bottomLeft,
		sf::Vector2f center);

	void Update(float dt) override;
	void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void BeginAnimation(float duration, float start = 0.f);
	void StopAnimation() { _animationRunning = false; }

	void SetColor(const sf::Color& color);
	const sf::Color& GetColor() const;
};