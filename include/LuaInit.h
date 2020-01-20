#pragma once


namespace sol
{
	class state;
}

//Initializes the standard Lua libraries
void LuaInit_Base(sol::state& lua);

//Binds required SFML features to Lua
void LuaInit_SFML(sol::state& lua);

//Binds required Thor features to Lua
void LuaInit_Thor(sol::state& lua);

//Binds engine related classes and functions to Lua
void LuaInit_Game(sol::state& lua);