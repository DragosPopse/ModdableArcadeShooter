#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

template <class K, class V>
class ResourceHolder
{
	std::map<K, std::unique_ptr<V>> _resources;
public:
	bool Load(const K& key, const std::string& file)
	{
		std::unique_ptr<V> resource(new V());
		bool success = resource.loadFromFile(file);
		if (success)
		{
			_resources.insert(key, std::move(resource));
		}
		return success;
	}

	
	void Release(const K& key)
	{
		_resources.erase(key);
	}


	V& operator[](const K& key)
	{
		auto it = _resources.find(key);
		return *it->second;
	}


	const V& operator[](const K& key)
	{
		auto it = _resources.find(key);
		return *it->second;
	}
};


typedef ResourceHolder<std::string, sf::Texture> TextureHolder;
typedef ResourceHolder<std::string, sf::Font> FontHolder;