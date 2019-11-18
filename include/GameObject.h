#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include "Context.h"

class Scene;
struct Command;
struct LuaCommand;

class GameObject :
	public sf::Transformable
{
	bool _active;
	bool _markedForDestroy;
	std::vector<std::unique_ptr<GameObject>> _children;
	std::vector<sol::userdata> _unownedChildren;
	GameObject* _parent;
	
public:
	enum Type
	{
		None = 0,
		PlayerAirplane = 1 << 0,
		EnemyAirplane = 1 << 1,
		PlayerProjectile = 1 << 2,
		EnemyProjectile = 1 << 3,
		PickupItem = 1 << 4,
		AnimationType = 1 << 5,
		TextType = 1 << 6
	};

	GameObject();
	virtual ~GameObject();

	//this should be called when the scene is set
	virtual void Start(Scene* scene);

	virtual void Update(float dt);
	virtual void FixedUpdate(float dt);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void OnCommand(const Command& command, float dt);
	void OnLuaCommand(const LuaCommand& command, float dt);

	void AddChild(std::unique_ptr<GameObject> obj);
	void AddUnownedChild(sol::userdata obj);
	std::unique_ptr<GameObject> RemoveChild(const GameObject& obj);
	
	void RemoveDestroyedChilldren();

	bool IsActive() const;

	void SetActive(bool active);

	sf::Transform GetWorldTransform() const;

	sf::Vector2f GetWorldPosition() const;

	void MarkForDestroy();

	bool IsDestroyed() const;

	virtual unsigned int GetCategory() const { return None; }

	virtual sf::FloatRect GetBoundingRect() const { return sf::FloatRect(0, 0, 0, 0); }
};