/*
	SoundQueue globalizes the sound playing so that you don't need to store individual
sf::Sound objects in each GameObject
	SoundQueue will always destroy the front element if it is not playing
	Use case: Airplane instantiates an explosion and then destroys itself. Airplane can't store the sf::Sound and
the explosion is a generic Animation that is not supposed to store an sf::Sound
	Benchmarking should be done to see if std::list performs better than std::deque. Speaking of framerate there isn't any noticeable difference
*/

#pragma once

#include <list> 

#include <SFML/Audio.hpp>

class SoundQueue
{
	std::list<sf::Sound> _queue;

public:
	using Iterator = decltype(_queue)::iterator;

	//Must start with lowercase for the class to support foreach loop
	Iterator begin();
	Iterator end();

	void PlaySound(const sf::SoundBuffer& buffer, float volume, float pitch = 1);

	void Update();
};