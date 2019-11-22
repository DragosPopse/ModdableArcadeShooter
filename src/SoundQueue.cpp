#include "SoundQueue.h"


void SoundQueue::Push(const sf::SoundBuffer& buffer, float pitch, float volume)
{
	sf::Sound sound(buffer);
	sound.setPitch(pitch);
	sound.setVolume(volume);
	auto& front = _queue.front();
	front.play();
}


void SoundQueue::Update()
{
	if (_queue.front().getStatus() == sf::Sound::Stopped)
	{
		_queue.pop_front();
	}
}