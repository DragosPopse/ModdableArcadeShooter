#include "GameObjects/CircleCooldown.h"

#include <algorithm>
#include <cmath>
#include <iostream>


CircleCooldown::CircleCooldown() :
	_elapsedTime(2),
	_duration(1),
	_animationRunning(false),
	_vertices(sf::TrianglesFan)
{
}


void CircleCooldown::SetVertices(sf::Vector2f topLeft,
	sf::Vector2f topRight,
	sf::Vector2f bottomRight,
	sf::Vector2f bottomLeft,
	sf::Vector2f center)
{
	_topLeft = topLeft;
	_topRight = topRight;
	_bottomRight = bottomRight;
	_bottomLeft = bottomLeft;
	_center = center;
}


void CircleCooldown::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (_animationRunning)
	{
		target.draw(_vertices, sf::RenderStates::Default);
	}

	GameObject::Draw(target, states);
}


void CircleCooldown::Update(float dt)
{
	if (_animationRunning)
	{
		_elapsedTime += dt;
		_vertices.clear();
		_progress = 1 - _elapsedTime / _duration;
		if (_progress < 0)
		{
			_progress = 0;
			_animationRunning = false;
		}
		float angle = M_PI * 2 * _progress - M_PI / 2;
		float x = cos(angle);
		float y = sin(angle);
		float l = std::max(abs(x), abs(y));
		x /= l;
		y /= l;
		_vertices.append(sf::Vertex(_center));
		_vertices.append(sf::Vertex(sf::Vector2f(_center.x, _topLeft.y)));
		if (_progress >= 0.125) _vertices.append(sf::Vertex(_topRight));
		if (_progress >= 0.375) _vertices.append(sf::Vertex(_bottomRight));
		if (_progress >= 0.625) _vertices.append(sf::Vertex(_bottomLeft));
		if (_progress >= 0.875) _vertices.append(sf::Vertex(_topLeft));

		_vertices.append(sf::Vertex(sf::Vector2f(_center.x + x * (_bottomRight.x - _topLeft.x) / 2, _center.y + y * (_bottomRight.y - _topLeft.y) / 2)));

		for (int i = 0; i < _vertices.getVertexCount(); i++)
		{
			_vertices[i].color = sf::Color(0, 0, 0, 100);
		}
	}
}


void CircleCooldown::BeginAnimation(float duration)
{
	_duration = duration;
	_elapsedTime = 0;
	_animationRunning = true;
}