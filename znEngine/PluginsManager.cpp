#include "stdafx.h"

// General
#include "PluginsManager.h"



typedef IznPlugin*(GetPluginFuncProc)(IBaseManager* BaseManager);


CznPluginsManager::CznPluginsManager(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CznPluginsManager::~CznPluginsManager()
{
	for (const auto& pluginPair : m_Plugins)
	{
		RemovePlugin(pluginPair.second);
	}

	m_Plugins.clear();
}



//
// IznPluginsManager
//
bool CznPluginsManager::AddPlugin(const std::string& PluginDLLName)
{
	try
	{
		//if (m_Plugins.find(PluginDLLName) != m_Plugins.end())
		//{
		//	throw CPluginException("Already registered.");
		//}

		char modulePath[MAX_PATH];
		::GetModuleFileNameA(NULL, modulePath, MAX_PATH);

		std::string modulePathStr(modulePath);
		modulePathStr = modulePathStr.substr(0, modulePathStr.find_last_of("\\") + 1);

		HMODULE pluginDLL = LoadLibraryA(PluginDLLName.c_str());
		if (pluginDLL == NULL)
		{
			DWORD lastError = ::GetLastError();
			throw CPluginException_NotAPlguin("Error while ::LoadLibrary().");
		}

		void* getPluginProcNative = ::GetProcAddress(pluginDLL, "GetPlugin");
		if (getPluginProcNative == NULL)
		{
			FreeLibrary(pluginDLL);
			return false;
		}

		std::shared_ptr<IznPlugin> pluginObject(((GetPluginFuncProc*)getPluginProcNative)(m_BaseManager));
		if (pluginObject == nullptr)
		{
			FreeLibrary(pluginDLL);
			throw CPluginException("Error while create plugin object.");
		}

		m_Plugins.push_back(std::make_pair(pluginDLL, pluginObject));

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
	const auto& iter = std::find_if(m_Plugins.begin(), m_Plugins.end(), [&Plugin](const std::pair<HMODULE, std::shared_ptr<IznPlugin>>& Plugin0) -> bool { return Plugin0.second.get() == Plugin.get(); });
	if (iter == m_Plugins.end())
	{
		Log::Warn("Plguin not found.");
		return;
	}

	iter->second->Finalize();

	// Notify all listeners about finalize plguin
	for (const auto& listener : m_PluginsEventsListener)
		listener->OnPluginFinalized(iter->second);

	if (::FreeLibrary(iter->first) == FALSE)
		_ASSERT_EXPR(false, L"CznPluginsManager: Failed to '::FreeLibrary()'.");

	iter->second.reset();
}

void CznPluginsManager::InitializeAllPlugins()
{
	for (const auto& pluginPair : m_Plugins)
	{
		const std::shared_ptr<IznPlugin>& plugin = pluginPair.second;

		if (!plugin->Initialize())
			throw CPluginException("Error while initialize.");

		// Notify all listeners about initialize plguin
		for (const auto& listener : m_PluginsEventsListener)
			listener->OnPluginInitialized(plugin);
	}
}

void CznPluginsManager::AddPluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{
	m_PluginsEventsListener.push_back(PluginEventListener);
}

void CznPluginsManager::RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{
	const auto& iter = std::find(m_PluginsEventsListener.begin(), m_PluginsEventsListener.end(), PluginEventListener);
	if (iter == m_PluginsEventsListener.end())
	{
		Log::Warn("Unable to remove plugin event listeneer.");
		return;
	}

	m_PluginsEventsListener.erase(iter);
}
