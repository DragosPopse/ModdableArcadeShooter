	#include "LuaInit.h"
#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Airplane.h"
#include "Utility.h"



void LuaInit_Base(sol::state& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::math);
}


void LuaInit_SFML(sol::state& lua)
{
	sf::Vector2f(*v2fMultiplication)(const sf::Vector2f&, float) = &sf::operator*;
	sf::Vector2f(*v2fMultiplicationRev)(float, const sf::Vector2f&) = &sf::operator*;
	sf::Vector2f(*v2fAddition)(const sf::Vector2f&, const sf::Vector2f&) = &sf::operator+;


	sol::table sfTable = lua.create_named_table("sf");
	sfTable.new_usertype<sf::Vector2f>("Vector2f",
		sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>(),
		"x", &sf::Vector2f::x,
		"y", &sf::Vector2f::y,
		sol::meta_function::multiplication, v2fMultiplication,
		sol::meta_function::multiplication, v2fMultiplicationRev,
		sol::meta_function::addition, v2fAddition
		);
	sfTable.get<sol::table>("Vector2f").set_function("normalize", &Normalize);
	sfTable.get<sol::table>("Vector2f").set_function("magnitude", &Magnitude);
}


void LuaInit_Game(sol::state& lua)
{
	sol::table engineTable = lua.create_named_table("engine");

	engineTable.set_function("toDegree", &ToDegree);
	engineTable.set_function("toRadian", &ToRadian);
	
	engineTable.new_usertype<Projectile>("Projectile",
		"getSpeed", &Projectile::GetSpeed,
		"getDamage", &Projectile::GetDamage,
		"setVelocity", &Projectile::SetVelocity,
		"getVelocity", &Projectile::GetVelocity,
		"destroy", &Projectile::MarkForDestroy
		);

	engineTable.new_usertype<Airplane>("Airplane",
		"damage", &Airplane::Damage,
		"repair", &Airplane::Repair);

	engineTable.new_usertype<Projectile::Context>("ProjectileContext",
		"deltaTime", &Projectile::Context::deltaTime);
}