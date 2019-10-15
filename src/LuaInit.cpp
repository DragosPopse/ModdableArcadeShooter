	#include "LuaInit.h"
#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Airplane.h"
#include "Utility.h"
#include <functional>
#include "Command.h"
#include "Scenes/Level.h"


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
	engineTable.set_function("distance", &Distance);
	engineTable.set_function("direction", &Direction);
	
	engineTable.new_usertype<Projectile>("Projectile",
		"getSpeed", &Projectile::GetSpeed,
		"getDamage", &Projectile::GetDamage,
		"setVelocity", &Projectile::SetVelocity,
		"getVelocity", &Projectile::GetVelocity,
		"destroy", &Projectile::MarkForDestroy,
		"setRotation", &Projectile::setRotation,
		"getRotation", &Projectile::getRotation,
		"isPlayerControlled", &Projectile::IsPlayerControlled,
		"getWorldPosition", &Projectile::GetWorldPosition,
		"getLevel", &Projectile::GetLevel
		);

	engineTable.new_usertype<Airplane>("Airplane",
		"damage", &Airplane::Damage,
		"repair", &Airplane::Repair,
		"getHealth", &Airplane::GetHealth,
		"onCommand", &Airplane::OnLuaCommand,
		"getPosition", &Airplane::getPosition,
		"getWorldPosition", &Airplane::GetWorldPosition);

	engineTable.new_usertype<LuaCommand>("Command",
		"action", &LuaCommand::action,
		"category", &LuaCommand::category);

	engineTable.new_usertype<GameObject>("GameObject",
		"onCommand", &GameObject::OnLuaCommand);
	engineTable.get<sol::table>("GameObject").set("PLAYER_AIRPLANE", GameObject::PlayerAirplane);
	engineTable.get<sol::table>("GameObject").set("ENEMY_AIRPLANE", GameObject::EnemyAirplane);
	engineTable.get<sol::table>("GameObject").set("PLAYER_PROJECTILE", GameObject::PlayerProjectile);
	engineTable.get<sol::table>("GameObject").set("ENEMY_PROJECTILE", GameObject::EnemyProjectile);

	engineTable.new_usertype<Level>("Level",
		"getEnemyProjectilesRoot", &Level::GetEnemyProjectilesRoot,
		"getPlayerProjectilesRoot", &Level::GetPlayerProjectilesRoot,
		"getPlayerAirplane", &Level::GetPlayerAirplane,
		"getEnemyAirplanesRoot", &Level::GetEnemyAirplanesRoot);
}