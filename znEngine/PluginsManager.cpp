#include "stdafx.h"

// General
#include "PluginsManager.h"



typedef IznPlugin*(GetPluginFuncProc)(IBaseManager* BaseManager);


CznPluginsManager::CznPluginsManager(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CznPluginsManager::~CznPluginsManager()
{}



//
// IznPluginsManager
//
bool CznPluginsManager::AddPlugin(const std::string& PluginDLLName)
{
	try
	{
		if (m_Plugins.find(PluginDLLName) != m_Plugins.end())
		{
			throw CPluginException("Already registered.");
		}

		HMODULE pluginDLL = LoadLibraryA(PluginDLLName.c_str());
		if (pluginDLL == NULL)
		{
			throw CPluginException_NotAPlguin("File not found.");
		}

		void* getPluginProcNative = GetProcAddress(pluginDLL, "GetPlugin");
		if (getPluginProcNative == NULL)
		{
			throw CPluginException_NotAPlguin("Is not a ZenonEngine plguin.");
		}

		GetPluginFuncProc* getPluginProc = (GetPluginFuncProc*)getPluginProcNative;

		std::shared_ptr<IznPlugin> pluginObject(getPluginProc(m_BaseManager));
		if (pluginObject == nullptr)
		{
			throw CPluginException("Error while create plugin object.");
		}

		m_Plugins.insert(std::make_pair(PluginDLLName, pluginObject));

		// Notify all listeners about new plguin
		for (const auto& listener : m_PluginsEventsListener)
			listener->OnPluginAdded(pluginObject);
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

void CznPluginsManager::RemovePlugin(std::shared_ptr<IznPlugin> Plugin)
{
}

void CznPluginsManager::InitializeAllPlugins()
{
	for (const auto& pluginPair : m_Plugins)
	{
		const std::shared_ptr<IznPlugin> plugin = pluginPair.second;

		if (!plugin->Initialize())
			throw CPluginException("Error while initialize.");

		// Notify all listeners about initialize plguin
		for (const auto& listener : m_PluginsEventsListener)
			listener->OnPluginAdded(plugin);
	}
}

void CznPluginsManager::AddPluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{
	m_PluginsEventsListener.push_back(PluginEventListener);
}

void CznPluginsManager::RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{

}
