#include "Scenes/Level.h"
#include <string>
#include "Context.h"
#include <sol/sol.hpp>
#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/Projectile.h"
#include <sol/sol.hpp>
#include "Utility.h"


Level::Level(Context* context, const std::string& fileName) :
	Scene(context),
	_root(new GameObject())
{
	sol::table level = _context->lua->do_file(BuildString("assets/scripts/levels/", fileName));
	
		
}


bool Level::FixedUpdate(float dt)
{
	_root->RemoveDestroyedChilldren();
	return false;
}


bool Level::Update(float dt)
{
	return false;
}


bool Level::Render()
{
	return false;
}