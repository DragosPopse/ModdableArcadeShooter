#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Context.h"
#include "Component.h"


class GameObject : 
	public sf::Transformable
{
	bool _active;
	std::vector<std::unique_ptr<GameObject>> _children;
	GameObject* _parent;

	std::vector<Component*> _updateComponents;
	std::vector<Component*> _fixedUpdateComponents;
	std::vector<Component*> _drawComponents;
	ComponentBitSet _componentBitSet;
	ComponentArray _componentArray;
	
public:
	GameObject();
	virtual ~GameObject();

	void Start();
	
	void Update(float dt);
	void FixedUpdate(float dt);
	void Draw() const;

	void AddChild(std::unique_ptr<GameObject> obj);
	std::unique_ptr<GameObject> RemoveChild(const GameObject& obj);

	bool IsActive() const;

	void SetActive(bool active);

	sf::Transform GetWorldTransform() const;

	sf::Vector2f GetWorldPosition() const;

	template <class T, bool hasUpdate, bool hasFixedUpdate, bool hasDraw, class... Args>
	T& AddComponent(Args&& ... args)
	{
		T* comp = new T(std::forward<Args>(args)...);
		comp->_gameObject = this;
		std::unique_ptr<Component> ptr(comp);

		_componentArray[GetComponentTypeID<T>()] = std::move(ptr);
		_componentBitSet[GetComponentTypeID<T>()] = true;

		if (hasUpdate)
		{
			_updateComponents.push_back(comp);
		}
		if (hasFixedUpdate)
		{
			_fixedUpdateComponents.push_back(comp);
		}
		if (hasDraw)
		{
			_drawComponents.push_back(comp);
		}

		comp->Init();

		return *comp;
	}


	template <class T>
	bool HasComponent() const
	{
		return _componentBitSet[GetComponentTypeID<T>()];
	}


	template <class T> 
	T& GetComponent() const
	{
		auto ptr(_componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};