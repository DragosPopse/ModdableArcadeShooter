#pragma once


#include <SFML/Audio.hpp>
#include <random>

struct RandomizedSoundResult
{
	sf::SoundBuffer* buffer;
	float volume;
	float pitch;
};

class RandomizedSound
{
	sf::SoundBuffer* _buffer;
	std::uniform_real_distribution<float> _volumeDistribution;
	std::uniform_real_distribution<float> _pitchDistribution;
	
public:
	RandomizedSound();

	void SetVolumeFactorDistribution(float min, float max);
	void SetPitchDistribution(float min, float max);
	void SetBuffer(sf::SoundBuffer* buffer);

	template <class Engine>
	RandomizedSoundResult operator()(Engine& engine, float defaultVolume)
	{
		RandomizedSoundResult result;
		result.buffer = _buffer;
		result.volume = _volumeDistribution(engine) * defaultVolume;
		result.pitch = _pitchDistribution(engine);

		return result;
	}
};