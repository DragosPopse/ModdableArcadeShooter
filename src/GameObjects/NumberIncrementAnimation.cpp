#include "GameObjects/NumberIncrementAnimation.h"
#include "Utility.h"


NumberIncrementAnimation::NumberIncrementAnimation() :
	_currentState(nullptr),
	_incrementState(this),
	_scaleState(this)
{
	SetNumber(0);
}


void NumberIncrementAnimation::Start(Scene* scene) 
{
	_currentState = &_incrementState; 
	_currentIncrement = _increments.cbegin();
	_incrementState.Start();
}


void NumberIncrementAnimation::Update(float dt)
{
	if (_currentState)
	{
		_currentState->Update(dt);
	}
	
	GameObject::Update(dt);
}


NumberIncrementAnimation::AnimationState::AnimationState(NumberIncrementAnimation* animation) :
	_animation(animation)
{
}


NumberIncrementAnimation::IncrementState::IncrementState(NumberIncrementAnimation* animation) :
	AnimationState(animation),
	_elapsedTime(0.f),
	_currentNumber(0)
{
}


NumberIncrementAnimation::ScaleState::ScaleState(NumberIncrementAnimation* animation) :
	AnimationState(animation),
	_elapsedTime(0.f)
{
}


NumberIncrementAnimation::StateID NumberIncrementAnimation::IncrementState::GetStateID() const
{
	return StateID::Increment;
}


NumberIncrementAnimation::StateID NumberIncrementAnimation::ScaleState::GetStateID() const
{
	return StateID::Scale;
}


void NumberIncrementAnimation::IncrementState::Start()
{
	_elapsedTime = 0.f;
	_targetNumber = *_animation->_currentIncrement;
	_beginNumber = _currentNumber;
	std::cout << "TARGET: " << _beginNumber << '\n';
	++_animation->_currentIncrement;
}


void NumberIncrementAnimation::ScaleState::Start()
{
	_elapsedTime = 0.f;
	_upscaling = true;
	_beginCharSize = _animation->GetCharSize();
}


void NumberIncrementAnimation::IncrementState::Update(float dt)
{
	_elapsedTime += dt;
	float progress = _elapsedTime / _incrementDuration;
	
	if (progress <= 1.f)
	{
		_currentNumber = Lerp(_beginNumber, _targetNumber, progress);
	}
	else
	{
		_currentNumber = _targetNumber;
		_animation->PushScaleState();
	}
	_animation->SetNumber(_currentNumber);
}


void NumberIncrementAnimation::ScaleState::Update(float dt)
{
	_elapsedTime += dt;
	float progress = _elapsedTime / _scaleDuration;

	if (_upscaling)
	{
		if (progress <= 1.f)
		{
			_currentCharSize = Lerp(_beginCharSize, _endCharSize, progress);
		}
		else
		{
			_currentCharSize = _endCharSize;
			_upscaling = false;
			_elapsedTime = 0.f;
		}
	}
	else
	{
		if (progress <= 1.f)
		{
			_currentCharSize = Lerp(_endCharSize, _beginCharSize, progress);
		}
		else
		{
			_currentCharSize = _beginCharSize;
			
			if (_animation->_currentIncrement != _animation->_increments.cend())
			{
				_animation->PushIncrementState();
			}
			else
			{
				_animation->_currentState = nullptr;
			}
		}
	}

	_animation->SetCharSize(_currentCharSize);
}


void NumberIncrementAnimation::SetNumber(int n)
{
	SetString(std::to_string(n));
}


void NumberIncrementAnimation::PushIncrementState()
{
	_currentState = &_incrementState;
	_incrementState.Start();
}


void NumberIncrementAnimation::PushScaleState()
{
	_currentState = &_scaleState;
	_scaleState.Start();
}


void NumberIncrementAnimation::SetIncrementDuration(float t)
{
	_incrementState.SetIncrementDuration(t);
}


void NumberIncrementAnimation::IncrementState::SetIncrementDuration(float t)
{
	_incrementDuration = t;
}


void NumberIncrementAnimation::SetFinalCharSize(size_t size)
{
	_scaleState.SetFinalCharSize(size);
}


void NumberIncrementAnimation::ScaleState::SetFinalCharSize(size_t size)
{
	_endCharSize = size;
}


void NumberIncrementAnimation::SetScaleDuration(float t)
{
	_scaleState.SetScaleDuration(t);
}


void NumberIncrementAnimation::ScaleState::SetScaleDuration(float t)
{
	_scaleDuration = t;	
}


void NumberIncrementAnimation::AddIncrement(int n)
{
	_increments.push_back(n);
}