#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Context.h"


class GameObject :
	public sf::Transformable,
	public sf::Drawable,
	private sf::NonCopyable
{
	std::vector<std::unique_ptr<GameObject>> _children;
	GameObject* _parent;
	Context _context;
	
public:
	GameObject();
	virtual ~GameObject();

	void Update(float dt);
	void FixedUpdate(float dt);

	void AddChild(std::unique_ptr<GameObject> obj);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	
};