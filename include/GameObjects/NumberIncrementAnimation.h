/*
	Handles a particular kind of text animation
	The animation is visible on the score at the end of the game. Every increment is followed by a scaling animation
*/


#pragma once


#include "GameObject.h"
#include "TextObject.h"
#include <vector>


class NumberIncrementAnimation :
	public TextObject
{
public:
	enum class StateID
	{
		Increment,
		Scale
	};

private:
	/*
	The animation is implemented using the State Machine pattern
	Each stage of the animation will be represented by a "State" object

	*/
	class AnimationState
	{
	protected:
		NumberIncrementAnimation* _animation;

	public:
		AnimationState(NumberIncrementAnimation* animation);

		virtual void Start() = 0;

		virtual void Update(float dt) = 0;

		virtual StateID GetStateID() const = 0;
	};


	class IncrementState :
		public AnimationState
	{
		float _incrementDuration;
		float _elapsedTime;
		int _currentNumber;
		int _beginNumber;
		int _targetNumber;

	public:
		IncrementState(NumberIncrementAnimation* animation);

		void Start() override;

		void Update(float dt) override;

		StateID GetStateID() const override;

		void SetIncrementDuration(float t);
	};


	class ScaleState :
		public AnimationState
	{
		float _scaleDuration;
		size_t _endCharSize;
		size_t _beginCharSize;
		size_t _currentCharSize;
		float _elapsedTime;
		bool _upscaling;

	public:
		ScaleState(NumberIncrementAnimation* animation);

		void Start() override;

		void Update(float dt) override;

		StateID GetStateID() const override;

		void SetFinalCharSize(size_t size);
		void SetScaleDuration(float t);
	};

	
	AnimationState* _currentState;
	IncrementState _incrementState;
	ScaleState _scaleState;

	std::vector<int> _increments;
	std::vector<int>::const_iterator _currentIncrement;

public:

	NumberIncrementAnimation();


	//Animation initialization
	void SetIncrementDuration(float t);
	void SetScaleDuration(float t); // where t equals 0.5 of the animation since the animation involves both upscaling and downscaling
	void SetFinalCharSize(size_t size);
	void AddIncrement(int n);

	//Overrides from GameObject
	void Start(Scene* scene) override;
	void Update(float dt) override;
	//void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void SetNumber(int n);
	void PushIncrementState();
	void PushScaleState();
};