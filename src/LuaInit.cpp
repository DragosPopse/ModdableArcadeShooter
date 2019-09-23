#include "LuaInit.h"
#include <sol/sol.hpp>


void LuaInit_Base(sol::state& lua)
{
	lua.open_libraries(sol::lib::base | sol::lib::math);
}


void LuaInit_SFML(sol::state& lua)
{

}


void LuaInit_Game(sol::state& lua)
{

}