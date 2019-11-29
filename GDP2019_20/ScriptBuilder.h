#pragma once
#include <map>
#include <string>

extern "C" {
#include <Lua5.3.5/lua.h>
#include <Lua5.3.5/lauxlib.h>
#include <Lua5.3.5/lualib.h>
}

#include "iCommand.h"
#include "GameItemFactory/iGameItem.h"

class ScriptBuilder
{
private:
	static std::map<std::string, iCommand*> loadedCommands;
	static iCommand* finalScript;

	static aGameItem* findItem(std::string name);
	static void printLoadedCommands();
public:
	static iCommand* TestScript();
	static iCommand* getFinalScript();

	static int chooseFinalScript(lua_State* L);
	static int BuildMoveCommand(lua_State* L);
	static int BuildSerialCommand(lua_State* L);
	static int addToSerial(lua_State* L);
	static int BuildParallelCommand(lua_State* L);
	static int addToParallel(lua_State* L);
	static int BuildRotateCommand(lua_State* L);
	static int BuildLookToward(lua_State* L);
	static int BuildFollowCommand(lua_State* L);
	static int BuildRideCurve(lua_State* L);
	static int BuildTriggerCommand(lua_State* L);
	static int BuildLookAt(lua_State* L);
};

