#pragma once

/*
	Base class for objects in the scene
*/

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
protected:
	bool _active;
	bool _markedForDestroy;
	std::vector<std::unique_ptr<GameObject>> _children;
	std::vector<sol::userdata> _unownedChildren;
	GameObject* _parent;
	
public:
	//GameObject types that support Commands
	enum class Type
	{
		None = 0,
		PlayerAirplane = 1 << 0,
		EnemyAirplane = 1 << 1,
		PlayerProjectile = 1 << 2,
		EnemyProjectile = 1 << 3,
		Pickup = 1 << 4,
		Animation = 1 << 5,
		Text = 1 << 6
	};

	GameObject();

	//Should be virtual because GameObject is a base class
	virtual ~GameObject();

	//this should be called when the scene is set
	virtual void Start(Scene* scene);

	//Called every frame
	virtual void Update(float dt);

	//Called a fixed number of times per second, depends on the implementation of Engine
	virtual void FixedUpdate(float dt);

	//Called every frame
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Passes the command recursively through the graph
	void OnCommand(const Command& command, float dt);

	//Same as OnCommand, but can be called from Lua code
	void OnLuaCommand(const LuaCommand& command, float dt);

	//Appends a GameObject to the graph with this as it's parent
	void AddChild(std::unique_ptr<GameObject> obj);

	//Used for adding GameObjects instantiated from Lua. Lua uses Garbage Collection so it controls it's own memory
	void AddUnownedChild(sol::userdata obj);
	std::unique_ptr<GameObject> RemoveChild(const GameObject& obj);
	
	//Deletes recursively every GameObject marked for destroy
	void RemoveDestroyedChilldren();

	bool IsActive() const;

	void SetActive(bool active);

	//Calculates the transform based on the parents
	sf::Transform GetWorldTransform() const;

	//Returns the position taking into acount the parents' transforms
	sf::Vector2f GetWorldPosition() const;

	//Marks the object ready to be removed from the graph
	virtual void MarkForDestroy();

	//Checks if the object is marked for destroy
	bool IsDestroyed() const;

	//Returns the GameObject Type. Defaults to Type::None
	virtual unsigned int GetCategory() const { return static_cast<unsigned int>(Type::None); }

	//Returns the bounding rectangle of the GameObject. Used for collisions
	virtual sf::FloatRect GetBoundingRect() const { return sf::FloatRect(0, 0, 0, 0); }
};