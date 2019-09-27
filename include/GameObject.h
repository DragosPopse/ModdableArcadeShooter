#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Context.h"

class Scene;


class GameObject :
	public sf::Transformable
{
	bool _active;
	bool _markedForDestroy;
	std::vector<std::unique_ptr<GameObject>> _children;
	GameObject* _parent;
	
public:
	GameObject();
	virtual ~GameObject();

	//this should be called when the scene is set
	virtual void Start(Scene* scene);

	virtual void Update(float dt);
	virtual void FixedUpdate(float dt);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void AddChild(std::unique_ptr<GameObject> obj);
	std::unique_ptr<GameObject> RemoveChild(const GameObject& obj);
	
	void RemoveDestroyedChilldren();

	bool IsActive() const;

	void SetActive(bool active);

	sf::Transform GetWorldTransform() const;

	sf::Vector2f GetWorldPosition() const;

	void MarkForDestroy();

	bool IsDestroyed() const;
};