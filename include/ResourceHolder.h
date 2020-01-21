#pragma once

/*
	Container for storing any kind of SFML resource. This includes sf::SoundBuffer, sf::Texture, sf::Font
	Many SFML tutorials give this implementation so I'm not sure who to credit here, it is pretty standard. 
*/

#include <map>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

template <class Key, class Resource>
class ResourceHolder
{
	std::map<Key, std::unique_ptr<Resource>> _resources;
public:
	bool Load(const Key& key, const std::string& file)
	{
		std::unique_ptr<Resource> resource(new Resource());
		bool success = resource->loadFromFile(file);
		if (success)
		{
			_resources.insert(std::make_pair(key, std::move(resource)));
		}
		return success;
	}

	
	void Release(const Key& key)
	{
		_resources.erase(key);
	}

	Resource& operator[](const Key& key)
	{
		auto it = _resources.find(key);
		return *it->second;
	}
};


using TextureHolder = ResourceHolder<std::string, sf::Texture>;
using FontHolder = ResourceHolder<std::string, sf::Font>;
using SoundHolder = ResourceHolder<std::string, sf::SoundBuffer>;