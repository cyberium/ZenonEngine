#include "stdafx.h"

// Include
#include <lua/lua.hpp>

// General
#include "LuaCore.h"

// Additional
#include <luabind/luabind.hpp>

int Func(lua_State * state)
{
    return 0;
}

CScriptCore::CScriptCore(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{
    m_LuaState = luaL_newstate();
    luaL_openlibs(m_LuaState);
    luabind::open(m_LuaState);

    std::shared_ptr<IFile> file = m_BaseManager->GetManager<IFilesManager>()->Open("test.lua");
    _ASSERT(file != NULL);


    lua_register(m_LuaState, "Func", Func);

    
    //
    // Qwerty
    luaL_dostring(m_LuaState, (const char*)file->getData());
    // Qwerty
    //


    lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);

    // перебираем все глобальные переменные (и функции там же лежат)
    
    /* const luabind::object& objects = luabind::globals(m_LuaState);
    for (luabind::iterator itr(objects); itr != luabind::iterator(); itr++)
    {
        const luabind::object& obj = (*itr);
        if (luabind::type(obj) == LUA_TTABLE) // это метатаблица
        {
            // перебираем данные в этой таблице
            for (luabind::iterator itr2(obj); itr2 != luabind::iterator(); itr2++)
            {
                // это переменная, хранящая имя файла со спрайтом?
                if (luabind::object_cast<std::string>(itr2.key()).compare("sprite") == 0)
                {
                    luabind::object key = itr.key(); // берём ключ
                    std::string name = luabind::object_cast<std::string>(key);
                    Log::Green("Name = %s", name.c_str());
                }
            }
        }
    }  */
}

CScriptCore::~CScriptCore()
{
    lua_close(m_LuaState);
}
