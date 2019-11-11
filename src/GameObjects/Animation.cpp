#include "GameObjects/Animation.h"
#include "Utility.h"
#include <iostream>


Animation::Animation() :
	_loop(false),
	_destroyOnFinish(false),
	_finished(false),
	_currentIndex(0),
	_elapsedTime(0)
{

}


void Animation::Update(float dt)
{
	//std::cout << GetWorldPosition().x << ' ' << GetWorldPosition().y << '\n';
	_elapsedTime += dt;
	if (_elapsedTime > _timePerFrame)
	{
		_elapsedTime = 0;
		_currentIndex++;
		if (_currentIndex >= _numberOfFrames)
		{
			if (_loop)
			{
				_currentIndex = 0;
				_currentFrame = _firstFrame;
				_sprite.setTextureRect(_currentFrame);
			}
			else
			{
				_finished = true;
				if (_destroyOnFinish)
				{
					MarkForDestroy();
				}
			}
		}
		else
		{
			_currentFrame.left += _currentFrame.width;
			_sprite.setTextureRect(_currentFrame);
		}
	}
	GameObject::Update(dt);
}


void Animation::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_sprite, states);

	GameObject::Draw(target, states);
}


void Animation::SetFirstRect(const sf::IntRect& rect)
{
	_currentFrame = rect;
	_firstFrame = rect;
	_sprite.setTextureRect(rect);
	CenterOrigin(_sprite);
}


bool Animation::IsFinished() const
{
	return _finished;
}


void Animation::SetTimePerFrame(float time)
{
	_timePerFrame = time;
}


void Animation::SetDestroyOnFinish(bool b)
{
	_destroyOnFinish = b;
}


void Animation::SetLoopable(bool b)
{
	_loop = b;
}


void Animation::SetTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
}


void Animation::SetNumberOfFrames(int n)
{
	_numberOfFrames = n;
}


void Animation::SetIndex(int n)
{
	_currentIndex = n;
	_elapsedTime = 0;
}