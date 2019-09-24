#pragma once

#include <bitset>
#include <memory>
#include <array>
#include <algorithm>

class Component;
class GameObject;

typedef std::size_t ComponentTypeID;
constexpr std::size_t MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentBitSet;
typedef std::array<std::unique_ptr<Component>, MAX_COMPONENTS> ComponentArray;


inline ComponentTypeID GetComponentTypeID()
{
	static ComponentTypeID lastID = 0;
	return lastID++;
}


template <class T> inline ComponentTypeID GetComponentTypeID() noexcept
{
	static ComponentTypeID typeID = GetComponentTypeID();
	return typeID;
}


class Component
{
public:
	GameObject* _gameObject;

	virtual ~Component() {}

	virtual void Init() {}
	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void FixedUpdate(float dt) {}
	virtual void Draw() const {}
};