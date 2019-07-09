#pragma once

// FORWARD BEGIN
struct lua_State;
// FORWARD END

class CScriptCore
{
public:
                                                    CScriptCore();
    virtual                                         ~CScriptCore();

private:
    lua_State *                                     m_LuaState;
};