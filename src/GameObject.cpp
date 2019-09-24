#include "GameObject.h"
#include <cassert>


GameObject::GameObject() :
	_parent(nullptr),
	_active(true)
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
	for (auto component : _updateComponents)
	{
		component->Update(dt);
	}

	for (auto& child : _children)
	{
		child->Update(dt);
	}
}


void GameObject::FixedUpdate(float dt)
{
	for (auto component : _fixedUpdateComponents)
	{
		component->FixedUpdate(dt);
	}

	for (auto& child : _children)
	{
		child->FixedUpdate(dt);
	}
}


void GameObject::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (auto component : _drawComponents)
	{
		component->Draw(target, states);
	}

	for (auto& child : _children)
	{
		child->Draw(target, states);
	}
}


void GameObject::Start()
{
	for (auto& component : _componentArray)
	{
		if (component.get())
		{
			component->Start();
		}
	}

	for (auto& child : _children)
	{
		child->Start();
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