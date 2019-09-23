#pragma once


namespace sol
{
	class state;
}


void LuaInit_Base(sol::state& lua);

void LuaInit_SFML(sol::state& lua);

void LuaInit_Game(sol::state& lua);