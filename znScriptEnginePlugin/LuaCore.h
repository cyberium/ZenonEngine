#pragma once

// FORWARD BEGIN
struct lua_State;
// FORWARD END

class CScriptCore
{
public:
                                                    CScriptCore(std::shared_ptr<IBaseManager> BaseManager);
    virtual                                         ~CScriptCore();

private:
	std::shared_ptr<IBaseManager>					m_BaseManager;
    lua_State *                                     m_LuaState;
};