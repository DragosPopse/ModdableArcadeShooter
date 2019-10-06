#include "GameObject.h"
#include <cassert>
#include "Scene.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include "Command.h"


GameObject::GameObject() :
	_parent(nullptr),
	_active(true),
	_markedForDestroy(false)
{
}


GameObject::~GameObject()
{
	
}


sf::Transform GameObject::GetWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	
	for (const GameObject* obj = this; obj != nullptr; obj = obj->_parent)
	{
		transform = obj->getTransform() * transform;
	}

	return transform;
}


sf::Vector2f GameObject::GetWorldPosition() const
{
	return GetWorldTransform() * sf::Vector2f();
}


bool GameObject::IsActive() const
{
	return _active;
}


void GameObject::SetActive(bool active)
{
	_active = active;
}


void GameObject::Update(float dt)
{
	for (auto& child : _children)
	{
		child->Update(dt);
	}
}


void GameObject::FixedUpdate(float dt)
{
	for (auto& child : _children)
	{
		child->FixedUpdate(dt);
	}
}


void GameObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& child : _children)
	{
		child->Draw(target, states);
	}
}


void GameObject::Start(Scene* scene)
{
	for (auto& child : _children)
	{
		child->Start(scene);
	}
}


void GameObject::AddChild(std::unique_ptr<GameObject> obj)
{
	obj->_parent = this;
	_children.push_back(std::move(obj));
}


std::unique_ptr<GameObject> GameObject::RemoveChild(const GameObject& obj)
{
	auto found = std::find_if(_children.begin(), _children.end(),
		[&](std::unique_ptr<GameObject>& ptr)
		{
			return ptr.get() == &obj;
		});

	assert(found != _children.end());

	auto result = std::move(*found);
	result->_parent = nullptr;
	_children.erase(found);

	return result;
}


void GameObject::MarkForDestroy()
{
	_markedForDestroy = true;
}


bool GameObject::IsDestroyed() const
{
	return _markedForDestroy;
}


void GameObject::RemoveDestroyedChilldren()
{
	for (auto& it = _children.begin(); it != _children.end(); )
	{
		if ((*it)->IsDestroyed())
		{
			it = _children.erase(it);
			std::cout << '.';
		}
		else
		{
			(*it)->RemoveDestroyedChilldren();
			++it;
		}
	}
}


void GameObject::OnCommand(const Command& command, float dt)
{
	if (command.category & GetCategory())
	{
		command.action(*this, dt);
	}

	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->OnCommand(command, dt);
	}
}