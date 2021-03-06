/*
	Handles a particular kind of text animation
	The animation is visible on the score at the end of the game. Every increment is followed by a scaling animation
*/


#pragma once

#include "TextObject.h"

#include <vector>



class NumberIncrementAnimation :
	public TextObject
{
public:
	enum class StateID
	{
		None,
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
		bool _skip;

	public:
		AnimationState(NumberIncrementAnimation* animation);

		virtual void Start() = 0;

		virtual void Update(float dt) = 0;

		virtual StateID GetStateID() const = 0;

		void Skip();
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

		int GetCurrentNumber() const;
	};


	class ScaleState :
		public AnimationState
	{
		float _scaleDuration;
		unsigned int _endCharSize;
		unsigned int _beginCharSize;
		unsigned int _currentCharSize;
		float _elapsedTime;
		bool _upscaling;

	public:
		ScaleState(NumberIncrementAnimation* animation);

		void Start() override;

		void Update(float dt) override;

		StateID GetStateID() const override;

		void SetFinalCharSize(unsigned int size);
		void SetScaleDuration(float t);
	};

	
	AnimationState* _currentState;
	IncrementState _incrementState;
	ScaleState _scaleState;

	std::vector<int> _increments;
	std::vector<int>::const_iterator _currentIncrement;
	int _nCurrentIncrement;

public:

	NumberIncrementAnimation();


	//Animation initialization
	void SetIncrementDuration(float t);
	void SetScaleDuration(float t); // where t equals 0.5 of the animation since the animation involves both upscaling and downscaling
	void SetFinalCharSize(unsigned int size);
	void AddIncrement(int n);

	//Overrides from GameObject
	void Start(Scene* scene) override;
	void Update(float dt) override;

	StateID GetCurrentState() const;

	int GetCurrentNumber() const;
	int GetCurrentIncrement() const;

	void Skip();

private:
	void SetNumber(int n);
	void PushIncrementState();
	void PushScaleState();
};