#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

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


typedef ResourceHolder<std::string, sf::Texture> TextureHolder;
typedef ResourceHolder<std::string, sf::Font> FontHolder;