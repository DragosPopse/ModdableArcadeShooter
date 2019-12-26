#include "RandomizedSound.h"
#include "Random.h"


RandomizedSound::RandomizedSound() :
	_buffer(nullptr)
{
}


void RandomizedSound::SetVolumeFactorDistribution(float min, float max)
{
	_minVolumeFactor = min;
	_maxVolumeFactor = max;
}


void RandomizedSound::SetPitchDistribution(float min, float max)
{
	_minPitch = min;
	_maxPitch = max;
}


void RandomizedSound::SetBuffer(sf::SoundBuffer* buffer)
{
	_buffer = buffer;
}


RandomizedSoundResult RandomizedSound::operator()(float defaultVolume)
{
	RandomizedSoundResult result;
	result.buffer = _buffer;
	result.volume = RandReal(_minVolumeFactor, _maxVolumeFactor) * defaultVolume;
	result.pitch = RandReal(_minPitch, _maxPitch);

	return result;
}