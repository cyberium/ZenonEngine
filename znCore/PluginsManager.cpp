#include "stdafx.h"

// General
#include "PluginsManager.h"



typedef IznPlugin*(GetPluginFuncProc)(std::shared_ptr<IBaseManager> BaseManager);


CznPluginsManager::CznPluginsManager(std::shared_ptr<IBaseManager> BaseManager)
	: m_BaseManager(BaseManager)
{}

CznPluginsManager::~CznPluginsManager()
{}



//
// IznPluginsManager
//
bool CznPluginsManager::RegisterPlugin(const std::string& PluginDLLName)
{
	try
	{
		if (m_Plugins.find(PluginDLLName) != m_Plugins.end())
			throw CPluginException("Already registered.");

		HMODULE pluginDLL = LoadLibraryA(PluginDLLName.c_str());
		if (pluginDLL == NULL)
			throw CPluginException_NotAPlguin("File not found.");

		void* getPluginProcNative = GetProcAddress(pluginDLL, "GetPlugin");
		if (getPluginProcNative == NULL)
			throw CPluginException_NotAPlguin("'GetPlugin' proc not found.");

		GetPluginFuncProc* getPluginProc = (GetPluginFuncProc*)getPluginProcNative;

		std::shared_ptr<IznPlugin> pluginObject(getPluginProc(m_BaseManager));
		if (pluginObject == nullptr)
			throw CPluginException("Error while create plugin object.");

		if (!pluginObject->Initialize(m_BaseManager))
			throw CPluginException("Error while initialize plugin.");

		m_Plugins.insert(std::make_pair(PluginDLLName, pluginObject));
	}
	catch (const CPluginException_NotAPlguin& e)
	{
		Log::Warn(("Plugin[" + PluginDLLName + "]: " + e.what()).c_str());
		return false;
	}
	catch (const CPluginException& e)
	{
		throw std::exception(("Plugin[" + PluginDLLName + "]: " + e.what()).c_str());
	}

	Log::Green("Plugin[%s]: Successfully added.", PluginDLLName.c_str());

	return true;
}

void CznPluginsManager::UnregisterPlugin()
{
}
