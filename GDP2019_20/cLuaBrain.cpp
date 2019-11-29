#include "cLuaBrain.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "ScriptBuilder.h"

int KillAllHumans(lua_State* L)
{
	// There are 3 things on the stack: "Justin", 47, 3.14159 
	const char* name = lua_tostring(L, 1);	// get "Justin"		
	int age = lua_tonumber(L, 2);			// "Cat"
	float pi = lua_tonumber(L, 3);			// 3.14159

	std::cout << "KillAllHumans(): "
		<< name << ", " << age << ", " << pi << std::endl;
	return 0;
}

cLuaBrain::cLuaBrain()
{
	// Create new Lua state.
	// NOTE: this is common to ALL script in this case
	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					/* Lua 5.3.3 */

	lua_pushcfunction( this->m_pLuaState, ScriptBuilder::BuildMoveCommand);
	lua_setglobal( this->m_pLuaState, "BuildMoveCommand" );

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::chooseFinalScript);
	lua_setglobal(this->m_pLuaState, "chooseFinalScript");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildSerialCommand);
	lua_setglobal(this->m_pLuaState, "BuildSerialCommand");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::addToSerial);
	lua_setglobal(this->m_pLuaState, "addToSerial");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildParallelCommand);
	lua_setglobal(this->m_pLuaState, "BuildParallelCommand");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::addToParallel);
	lua_setglobal(this->m_pLuaState, "addToParallel");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildRotateCommand);
	lua_setglobal(this->m_pLuaState, "BuildRotateCommand");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildLookToward);
	lua_setglobal(this->m_pLuaState, "BuildLookToward");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildFollowCommand);
	lua_setglobal(this->m_pLuaState, "BuildFollowCommand");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildRideCurve);
	lua_setglobal(this->m_pLuaState, "BuildRideCurve");
	
	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildTriggerCommand);
	lua_setglobal(this->m_pLuaState, "BuildTriggerCommand");

	lua_pushcfunction(this->m_pLuaState, ScriptBuilder::BuildLookAt);
	lua_setglobal(this->m_pLuaState, "BuildLookAt");

	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}

const unsigned int MAXLINELENGTH = 65536;		// 16x1024

void cLuaBrain::RunFile(std::string theLuaScript)
{
	std::ifstream file(theLuaScript.c_str());

	if (!file.is_open()) {
		printf("Didn't found %s lua script\n", theLuaScript.c_str());
		exit(1);
	}

	std::string script;

	char pLineTemp[MAXLINELENGTH] = { 0 };
	while (file.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		script += tempString + "\n";
	}

	file.close();

	std::cout << "Lua read script: " << script << std::endl;

	int error = luaL_loadstring( this->m_pLuaState,
								script.c_str() );

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
					  0,	/* nargs: number of arguments pushed onto the lua stack */
					  0,	/* nresults: number of results that should be on stack at end*/
					  0);	/* errfunc: location, in stack, of error function.
							if 0, results are on top of stack. */
	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(this->m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

	}

	return;
}

std::string cLuaBrain::m_decodeLuaErrorToString( int error )
{
	switch ( error )
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error"; 
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}
