#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.3.5/lua.h>
#include <Lua5.3.5/lauxlib.h>
#include <Lua5.3.5/lualib.h>
}

#include <string>
#include <vector>
#include <map>

#include "iCommand.h"
#include "Scene.h"

class cLuaBrain
{
private:
	lua_State* m_pLuaState;

	std::string m_decodeLuaErrorToString( int error );
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	void RunFile(std::string theLuaScript);
};

#endif
