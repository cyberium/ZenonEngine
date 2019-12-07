#pragma once

// FORWARD BEGIN
struct lua_State;
// FORWARD END

class CScriptCore
{
public:
                                                    CScriptCore(IBaseManager* BaseManager);
    virtual                                         ~CScriptCore();

private:
	IBaseManager*					m_BaseManager;
    lua_State *                                     m_LuaState;
};