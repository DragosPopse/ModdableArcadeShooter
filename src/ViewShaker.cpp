#include "ViewShaker.h"


ViewShaker::ViewShaker() :
	_shaking(false),
	_duration(0.f),
	_elapsedTime(0.f)
{
	
}


void ViewShaker::SetView(sf::View* view)
{
	_view = view;
}


void ViewShaker::Shake(float amplitude, float duration)
{
	_shaking = true;
	_elapsedTime = 0.f;
	_amplitude = amplitude;
	_duration = duration;
	_originalPosition = _view->getCenter();
}


bool ViewShaker::IsShaking() const
{
	return _shaking;
}



void ViewShaker::Update(float dt)
{
	if (_shaking)
	{
		_elapsedTime += dt;
		if (_elapsedTime < _duration)
		{
			sf::Vector2f displacement = _distribution(_rng) * _amplitude;
			_view->move(displacement);
		}
		else
		{
			_shaking = false;
		}
	}
}