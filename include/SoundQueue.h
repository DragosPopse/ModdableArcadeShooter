/*
	SoundQueue globalizes the sound playing so that you don't need to store individual
sf::Sound objects in each GameObject
	SoundQueue will always destroy the front element if it is not playing
	Use case: Airplane instantiates an explosion and then destroys itself. Airplane can't store the sf::Sound and
the explosion is a generic Animation that is not supposed to store an sf::Sound
	Benchmarks should be made to see if std::list performs better than std::vector
*/

#pragma once

#include <SFML/Audio.hpp>
#include <list> 
#include <vector>


class SoundQueue
{
	std::list<sf::Sound> _queue;

public:
	void Push(const sf::SoundBuffer& buffer, float pitch, float volume);

	void Update();
};