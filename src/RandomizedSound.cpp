#include "RandomizedSound.h"


RandomizedSound::RandomizedSound() :
	_buffer(nullptr)
{
}


void RandomizedSound::SetVolumeFactorDistribution(float min, float max)
{
	_volumeDistribution = std::uniform_real_distribution<float>(min, max);
}


void RandomizedSound::SetPitchDistribution(float min, float max)
{
	_pitchDistribution = std::uniform_real_distribution<float>(min, max);
}


void RandomizedSound::SetBuffer(sf::SoundBuffer* buffer)
{
	_buffer = buffer;
}