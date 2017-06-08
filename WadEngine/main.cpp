#include "Engine.h"

#include <lua.hpp>
#include <iostream>

char *szLua =
"x = 8 "
"return ( x > 7 ) ";

int main()
{
	/*lua_State *lState;

	lState = luaL_newstate();
	luaL_openlibs(lState);

	int iStatus = luaL_loadstring(lState, szLua);
	if (iStatus)
	{
		std::cout << "Error: " << lua_tostring(lState, -1);
		return 1;
	}
	iStatus = lua_pcall(lState, 0, LUA_MULTRET, 0);
	if (iStatus)
	{
		std::cout << "Error: " << lua_tostring(lState, -1);
		return 1;
	}

	int iRet = (int)lua_toboolean(lState, -1);
	if (iRet)
	{
		std::cout << "Door opened!" << std::endl;
	}
	else
	{
		std::cout << "Door still closed." << std::endl;
	}

	lua_close(lState);*/

	Engine engine;
	if (engine.init())
		engine.mainLoop();
	return 0;
}