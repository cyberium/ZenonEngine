#include "stdafx.h"

// General
#include "PluginsManager.h"

// Additional
#include "BaseManager.h"

typedef IznPlugin* (*GetPluginFuncProc)(std::shared_ptr<IBaseManager> BaseManager);

CznPluginsManager::CznPluginsManager()
{
}

CznPluginsManager::~CznPluginsManager()
{
}



//
// IznPluginsManager
//
bool CznPluginsManager::RegisterPlugin(const std::string& PluginDLLName)
{
	HMODULE pluginDLL = LoadLibraryA(PluginDLLName.c_str());
	if (pluginDLL == NULL)
		throw std::exception(("Plugin dll " + PluginDLLName + " not found.").c_str());

	void* getPluginProcNative = GetProcAddress(pluginDLL, "GetPlugin");
	if (getPluginProcNative == NULL)
		throw std::exception(("Plugin proc 'GetPlugin' not found in " + PluginDLLName).c_str());

	GetPluginFuncProc getPluginProc = (GetPluginFuncProc)getPluginProcNative;

	std::shared_ptr<IznPlugin> pluginObject(getPluginProc(_BaseManager));
	if (pluginObject == nullptr)
		throw std::exception(("Error while creating plugin " + PluginDLLName).c_str());

	return true;
}

void CznPluginsManager::UnregisterPlugin()
{
}
