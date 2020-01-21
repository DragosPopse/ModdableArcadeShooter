#pragma once

/*
	Sound utility for randomizing the pitch and volume of SFX.
	It adds variety to the same sound
*/

#include <SFML/Audio.hpp>

struct RandomizedSoundResult
{
	sf::SoundBuffer* buffer;
	float volume;
	float pitch;
};

class RandomizedSound
{
	sf::SoundBuffer* _buffer;
	float _minVolumeFactor;
	float _maxVolumeFactor;
	float _minPitch;
	float _maxPitch;
	
public:
	RandomizedSound();

	void SetVolumeFactorDistribution(float min, float max);
	void SetPitchDistribution(float min, float max);
	void SetBuffer(sf::SoundBuffer* buffer);

	RandomizedSoundResult operator()(float defaultVolume);
};