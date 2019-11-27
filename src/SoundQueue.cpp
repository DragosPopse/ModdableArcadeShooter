#include "SoundQueue.h"


void SoundQueue::PlaySound(const sf::SoundBuffer& buffer, float volume, float pitch)
{
	sf::Sound sound(buffer);
	sound.setPitch(pitch);
	sound.setVolume(volume);
	_queue.push_back(sound);
	auto& back = _queue.back();

	back.play();
}


void SoundQueue::Update()
{
	if (!_queue.empty() && _queue.front().getStatus() == sf::Sound::Stopped)
	{
		_queue.pop_front();
	}
}


SoundQueue::Iterator SoundQueue::begin()
{
	return _queue.begin();
}


SoundQueue::Iterator SoundQueue::end()
{
	return _queue.end();
}