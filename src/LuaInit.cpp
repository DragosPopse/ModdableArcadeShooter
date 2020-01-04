#include "LuaInit.h"

#include <functional>

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <Thor/Input.hpp>
#include <Thor/Graphics.hpp>
#include <Thor/Animations.hpp>

#include "GameObject.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Airplane.h"
#include "GameObjects/ParticleSystemObject.h"

#include "Scenes/Level.h"

#include "Random.h"
#include "Utility/Math.h"
#include "Utility/Animations.h"
#include "Command.h"



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
		sol::meta_function::multiplication, sol::overload(v2fMultiplication, v2fMultiplicationRev),
		sol::meta_function::addition, v2fAddition
		);
	sfTable.get<sol::table>("Vector2f").set_function("normalize", &Normalize);
	sfTable.get<sol::table>("Vector2f").set_function("magnitude", &Magnitude);

	sfTable.new_usertype<sf::Time>("Time",
		"asSeconds", &sf::Time::asSeconds);

	sfTable.new_usertype<sf::Texture>("Texure");

	sfTable.set_function("seconds", &sf::seconds);

	sfTable.new_usertype<sf::IntRect>("IntRect",
		sol::constructors<sf::IntRect(), sf::IntRect(int, int, int, int)>(),
		"left", &sf::IntRect::left,
		"top", &sf::IntRect::top,
		"width", &sf::IntRect::width,
		"height", &sf::IntRect::height);

	sfTable.new_usertype<sf::Color>("Color",
		sol::constructors< sf::Color(sf::Uint8, sf::Uint8, sf::Uint8, sf::Uint8)>());
	
	sol::table glTable = lua.create_named_table("gl");
	glTable.new_usertype<sf::Glsl::Vec4>("Vec4",
		sol::constructors<sf::Glsl::Vec4(float, float, float, float)>());
}


void LuaInit_Thor(sol::state& lua)
{
	sol::table thorTable = lua.create_named_table("thor");

	thorTable.new_usertype<ParticleAnimationFunction>("ParticleAnimationFunction");
	thorTable.new_usertype<thor::Particle>("Particle");
	thorTable.new_usertype<thor::Distribution<float>>("FloatDistribution");
	thorTable.new_usertype<thor::Distribution<sf::Vector2f>>("Vector2fDistribution");
	thorTable.new_usertype<thor::Distribution<int>>("IntDistribution");
	thorTable.new_usertype<thor::Distribution<sf::Time>>("TimeDistribution",
		sol::constructors<thor::Distribution<sf::Time>(sf::Time)>());
	thorTable.new_usertype<thor::Distribution<sf::Color>>("ColorDistribution",
		sol::constructors<thor::Distribution<sf::Color>(sf::Color)>());
	
	thorTable.new_usertype<thor::UniversalEmitter>("UniversalEmitter",
		"setEmissionRate", &thor::UniversalEmitter::setEmissionRate,
		"setParticleVelocity", &thor::UniversalEmitter::setParticleVelocity, 
		"setParticleLifetime", &thor::UniversalEmitter::setParticleLifetime,
		"setParticleRotation", &thor::UniversalEmitter::setParticleRotation,
		"setParticleTextureIndex", &thor::UniversalEmitter::setParticleTextureIndex,
		"setParticleScale", &thor::UniversalEmitter::setParticleScale,
		"setParticleColor", &thor::UniversalEmitter::setParticleColor);
	thor::Connection(thor::ParticleSystem::*addEmitter2)(std::function<void(thor::EmissionInterface&, sf::Time)>) = &thor::ParticleSystem::addEmitter;
	thor::Connection(thor::ParticleSystem::*addEmitter1)(std::function<void(thor::EmissionInterface&, sf::Time)>, sf::Time) = &thor::ParticleSystem::addEmitter;
	thor::Connection(thor::ParticleSystem::*addAffector1)(std::function<void(thor::Particle&, sf::Time)>) = &thor::ParticleSystem::addAffector;
	//thor::Connection(thor::ParticleSystem::*addAffector2)(std::function<void(thor::Particle&, sf::Time)>, sf::Time) = &thor::ParticleSystem::addAffector;


	thorTable.new_usertype<thor::ParticleSystem>("ParticleSystem",
		"addEmitter", sol::overload(addEmitter1, addEmitter2),
		"setTexture", &thor::ParticleSystem::setTexture,
		"addTextureRect", &thor::ParticleSystem::addTextureRect,
		"addAffector", sol::overload(addAffector1),
		"clearEmitters", &thor::ParticleSystem::clearEmitters,
		"clearAffectors", &thor::ParticleSystem::clearAffectors);

	sol::table distTable = thorTable.create_named("Distributions");
	
	thor::Distribution<float>(*uniform1)(float, float) = &thor::Distributions::uniform;
	thor::Distribution<int>(*uniform2)(int, int) = &thor::Distributions::uniform;
	thor::Distribution<unsigned int>(*uniform4)(unsigned int, unsigned int) = &thor::Distributions::uniform;
	thor::Distribution<sf::Time>(*uniform3)(sf::Time, sf::Time) = &thor::Distributions::uniform;
	distTable.set_function("floatUniform", uniform1);
	distTable.set_function("intUniform", uniform2);
	distTable.set_function("timeUniform", uniform3);
	distTable.set_function("uintUniform", uniform4);

	
	distTable.set_function("circle", &thor::Distributions::circle);
	distTable.set_function("deflect", &thor::Distributions::deflect);
	distTable.set_function("rect", &thor::Distributions::rect);

	

	thorTable.new_usertype<thor::FadeAnimation>("FadeAnimation",
		sol::constructors<thor::FadeAnimation(float, float)>());

	thorTable.new_usertype<thor::AnimationAffector>("AnimationAffector",
		sol::constructors<thor::AnimationAffector(thor::FadeAnimation)>());

	thorTable.new_usertype<thor::ScaleAffector>("ScaleAffector",
		sol::constructors<thor::ScaleAffector(sf::Vector2f)>());

	thorTable.new_usertype<thor::TorqueAffector>("TorqueAffector",
		sol::constructors<thor::TorqueAffector(float)>());
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
		"getLevel", &Projectile::GetLevel,
		"addChild", &Projectile::AddUnownedChild
		);

	engineTable.new_usertype<Airplane>("Airplane",
		"damage", &Airplane::Damage,
		"repair", &Airplane::Repair,
		"getHealth", &Airplane::GetHealth,
		"onCommand", &Airplane::OnLuaCommand,
		"getPosition", &Airplane::getPosition,
		"getWorldPosition", &Airplane::GetWorldPosition,
		"addChild", &Airplane::AddUnownedChild,
		"addHealth", &Airplane::AddHealth,
		"addAmmo", &Airplane::AddAmmo,
		"getLevel", &Airplane::GetLevel,
		"addChild", &Airplane::AddUnownedChild);

	engineTable.new_usertype<LuaCommand>("Command",
		"action", &LuaCommand::action,
		"category", &LuaCommand::category);

	engineTable.new_usertype<GameObject>("GameObject",
		"onCommand", &GameObject::OnLuaCommand);
	engineTable.get<sol::table>("GameObject").set("PLAYER_AIRPLANE", static_cast<unsigned int>(GameObject::Type::PlayerAirplane));
	engineTable.get<sol::table>("GameObject").set("ENEMY_AIRPLANE", static_cast<unsigned int>(GameObject::Type::EnemyAirplane));
	engineTable.get<sol::table>("GameObject").set("PLAYER_PROJECTILE", static_cast<unsigned int>(GameObject::Type::PlayerProjectile));
	engineTable.get<sol::table>("GameObject").set("ENEMY_PROJECTILE", static_cast<unsigned int>(GameObject::Type::EnemyProjectile));

	engineTable.new_usertype<Level>("Level",
		"getEnemyProjectilesRoot", &Level::GetEnemyProjectilesRoot,
		"getPlayerProjectilesRoot", &Level::GetPlayerProjectilesRoot,
		"getPlayerAirplane", &Level::GetPlayerAirplane,
		"getEnemyAirplanesRoot", &Level::GetEnemyAirplanesRoot,
		"getTexture", &Level::GetTexture,
		"addParticles", &Level::AddLuaParticle,
		"playVignetteAnimation", &Level::PlayVignetteAnimation,
		"shakeScreen", &Level::ShakeScreen);

	engineTable.new_usertype<UniformVector2fDistribution>("UniformVector2fDistribution",
		sol::constructors<UniformVector2fDistribution(float, float)>());
	engineTable.get<sol::table>("UniformVector2fDistribution").set_function("create", &UniformVector2fDistribution::Create);

	void(ParticleSystemObject::*setPosition1)(float, float) = &ParticleSystemObject::setPosition;
	void(ParticleSystemObject::*setPosition2)(const sf::Vector2f&) = &ParticleSystemObject::setPosition;
	engineTable.new_usertype<ParticleSystemObject>("ParticleSystem",
		"setPosition", sol::overload(setPosition1, setPosition2),
		"setRotation", &ParticleSystemObject::setRotation,
		"setRemoveAfterLifetime", &ParticleSystemObject::SetRemoveAfterLifetime,
		"system", &ParticleSystemObject::system,
		"addEmitter", &ParticleSystemObject::AddEmitter);
}