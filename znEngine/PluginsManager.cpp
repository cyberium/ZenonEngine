#include "stdafx.h"

// General
#include "PluginsManager.h"



typedef IznPlugin*(GetPluginFuncProc)(IBaseManager& BaseManager);


CznPluginsManager::CznPluginsManager(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{}

CznPluginsManager::~CznPluginsManager()
{
	for (const auto& pluginDescription : m_Plugins)
	{
		RemovePlugin(pluginDescription.Path);
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
		SPluginDescription pluginDescription;
		pluginDescription.Path = PluginDLLName;

		pluginDescription.HModule = LoadLibraryA(pluginDescription.Path.c_str());
		if (pluginDescription.HModule == NULL)
		{
			DWORD lastError = ::GetLastError();
			throw CPluginException_NotAPlguin("Error while ::LoadLibrary().");
		}


		void* getPluginProcNative = ::GetProcAddress(pluginDescription.HModule, "GetPlugin");
		if (getPluginProcNative == NULL)
		{
			::FreeLibrary(pluginDescription.HModule);
			return false;
		}

		pluginDescription.Plugin = std::shared_ptr<IznPlugin>(((GetPluginFuncProc*)getPluginProcNative)(m_BaseManager));
		if (pluginDescription.Plugin == nullptr)
		{
			::FreeLibrary(pluginDescription.HModule);
			throw CPluginException("Error while create plugin object.");
		}

		m_Plugins.push_back(pluginDescription);

		// Notify all listeners about new plguin
		for (const auto& listener : m_PluginsEventsListener)
			listener->OnPluginAdded(pluginDescription.Plugin);
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

void CznPluginsManager::RemovePlugin(const std::string& PluginDLLName)
{
	const auto& iter = std::find_if(m_Plugins.begin(), m_Plugins.end(), [&PluginDLLName](const SPluginDescription& PluginDescription) -> bool { return PluginDescription.Path == PluginDLLName; });
	if (iter == m_Plugins.end())
	{
		Log::Warn("Plguin not found.");
		return;
	}

	iter->Plugin->Finalize();

	// Notify all listeners about finalize plguin
	for (const auto& listener : m_PluginsEventsListener)
		listener->OnPluginFinalized(iter->Plugin);

	iter->Plugin.reset();

	if (::FreeLibrary(iter->HModule) == FALSE)
		_ASSERT_EXPR(false, L"CznPluginsManager: Failed to '::FreeLibrary()'.");
}

void CznPluginsManager::InitializeAllPlugins()
{
	for (const auto& pluginDescription : m_Plugins)
	{
		const auto& plugin = pluginDescription.Plugin;

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
