//---------------------------------------------------------------------------------- 
//  FIleName : PiScript.cpp
//                     
//	Class Name:	PiScript Class
//
//  Desc: Lua Script 처리를 한다.
//
//----------------------------------------------------------------------------------
#include <windows.h>
#include ".\piscript.h"
#include ".\PiScriptLuaCommand.h"

PiScript::PiScript(void) : m_pLuaState(NULL)
{
	m_pLuaState = lua_open();
	lua_baselibopen(m_pLuaState);

	RegisterCommand(m_pLuaState);
}

PiScript::~PiScript(void)
{
	if (m_pLuaState != NULL)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}
}

//----------------------------------------------------------------------------------
// Name: RegisterFunction
// Desc: Lua에서 호출할 함수를 등록한다.
// Date: 2004. 5. 12	수요일
// Pres: HoDong Hong. in PRISTON Company Republic of Korea
//----------------------------------------------------------------------------------
void PiScript::RegisterCommand(lua_State* state)
{
	RegisterFunction(luaCommand_Begin, "Begin");
	RegisterFunction(luaCommand_End, "End");
	RegisterFunction(luaCommand_InitPos, "InitPos");
	RegisterFunction(luaCommand_InitColor, "InitColor");
	RegisterFunction(luaCommand_InitSize, "InitSize");

	RegisterFunction(luaCommand_InitMeshName, "InitMeshName");
	RegisterFunction(luaCommand_InitMaxFrame, "InitMaxFrame");
	RegisterFunction(luaCommand_InitLoop, "InitLoop");
	RegisterFunction(luaCommand_InitTextureName, "InitTextureName");
	RegisterFunction(luaCommand_InitAniTextureName, "InitAniTextureName");

	RegisterFunction(luaCommand_InitStartDelayTime, "InitStartDelayTime");
	RegisterFunction(luaCommand_InitEndTime, "InitEndTime");

	RegisterFunction(luaCommand_EventColor, "EventColor");
	RegisterFunction(luaCommand_EventFadeColor, "EventFadeColor");

	RegisterFunction(luaCommand_EventSize, "EventSize");
	RegisterFunction(luaCommand_EventFadeSize, "EventFadeSize");

	RegisterFunction(luaCommand_InitSpawnBoundingBox, "InitSpawnBoundingBox");
	RegisterFunction(luaCommand_InitSpawnBoundingSphere, "InitSpawnBoundingSphere");
	RegisterFunction(luaCommand_InitSpawnBoundingDoughnut, "InitSpawnBoundingDoughnut");

	RegisterFunction(luaCommand_InitParticleType, "InitParticleType");
	RegisterFunction(luaCommand_InitVelocityType, "InitVelocityType");

	RegisterFunction(luaCommand_InitParticleNum, "InitParticleNum");
	RegisterFunction(luaCommand_InitEmitRate, "InitEmitRate");
	RegisterFunction(luaCommand_InitVelocity, "InitVelocity");

	RegisterFunction(luaCommand_InitAxialPos, "InitAxialPos");
	RegisterFunction(luaCommand_InitBlendType, "InitBlendType");
}

//----------------------------------------------------------------------------------
// Name: RegisterFunction
// Desc: Lua에서 호출할 함수를 등록한다.
// Date: 2004. 5. 14	목요일
// Pres: HoDong Hong. in PRISTON Company Republic of Korea
//----------------------------------------------------------------------------------
void PiScript::RegisterFunction(lua_CFunction fn, const char* name)
{
	lua_pushstring(m_pLuaState, name);
	lua_pushlightuserdata(m_pLuaState, this);
	lua_pushcclosure(m_pLuaState, fn, 1);
	lua_settable(m_pLuaState, LUA_GLOBALSINDEX);
	lua_settop(m_pLuaState, 0);
}

//----------------------------------------------------------------------------------
// Name: Run
// Desc: Lua File을 Interpret 한다. (*.lua 파일을 읽어서 처리를 한다.)
// Date: 2004. 5. 12	수요일
// Pres: HoDong Hong. in PRISTON Company Republic of Korea
//----------------------------------------------------------------------------------
bool PiScript::RunScriptFile(const char* fileName)
{
	if (m_pLuaState == 0)
		return false;

	if (luaL_loadfile(m_pLuaState, fileName) || lua_pcall(m_pLuaState, 0, 0, 0))
	{
		MessageBox(NULL, lua_tostring(m_pLuaState, -1), "File: Lua Syntax Error", MB_OK);
		lua_pop(m_pLuaState, 1);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------
// Name: Run
// Desc: buffer의 스크립트를 처리한다. (한라인씩 처리를 할경우)
// Date: 2004. 5. 12	수요일
// Pres: HoDong Hong. in PRISTON Company Republic of Korea
//----------------------------------------------------------------------------------
bool PiScript::RunScriptBuffer(const char* buffer)
{
	if (m_pLuaState == 0)
		return false;

	if (luaL_loadbuffer(m_pLuaState, buffer, strlen(buffer), "line") || lua_pcall(m_pLuaState, 0, 0, 0))
	{
		MessageBox(NULL, lua_tostring(m_pLuaState, -1), "Buffer: Lua Syntax Error", MB_OK);
		lua_pop(m_pLuaState, 1);
		return false;
	}

	return true;
}
