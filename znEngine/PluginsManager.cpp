#include "stdafx.h"

// General
#include "PluginsManager.h"

CPluginsManager::CPluginsManager(IApplication& Application)
	: m_Application(Application)
{}

CPluginsManager::~CPluginsManager()
{

}



//
// IznPluginsManager
//
bool CPluginsManager::AddPlugin(const std::string& PluginDLLName)
{
	const IApplication_PlatformBase& applicationPluginSpecific = dynamic_cast<const IApplication_PlatformBase&>(m_Application);

	SPluginDescription pluginDescription;
	pluginDescription.Filename = PluginDLLName;
	pluginDescription.Plugin = std::shared_ptr<IznPlugin>(applicationPluginSpecific.LoadPlugin(PluginDLLName));

	m_Plugins.push_back(pluginDescription);

	// Notify all listeners about new plguin
	for (const auto& listener : m_PluginsEventsListener)
		listener->OnPluginAdded(pluginDescription.Plugin);

	Log::Green("Plugin '%s' successfully added.", PluginDLLName.c_str());

	return true;
}

void CPluginsManager::RemovePlugin(std::shared_ptr<IznPlugin> Plugin)
{
	

	const auto& iter = std::find_if(m_Plugins.begin(), m_Plugins.end(), [&Plugin](const SPluginDescription& PluginDescription) -> bool { return PluginDescription.Plugin == Plugin; });
	if (iter == m_Plugins.end())
		throw CException("Plguin '%s' not found.", Plugin->GetName().c_str());


}

void CPluginsManager::InitializeAllPlugins()
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

void CPluginsManager::FinalizeAllPlugins()
{
	for (auto rIt = m_Plugins.rbegin(); rIt != m_Plugins.rend(); rIt++)
	{
		const auto& plugin = (*rIt);
		try
		{
			plugin.Plugin->Finalize();

			// Notify all listeners about finalize plguin
			for (const auto& listener : m_PluginsEventsListener)
				listener->OnPluginFinalized(plugin.Plugin);
		}
		catch (const CException& e)
		{
			Log::Error("Error while removing plugin '%s'.", plugin.Plugin->GetName().c_str());
			Log::Error("--->%s", e.MessageCStr());
		}
	}
}

void CPluginsManager::RemoveAllPlugins()
{
	const IApplication_PlatformBase& applicationPluginSpecific = dynamic_cast<const IApplication_PlatformBase&>(m_Application);

	for (auto rIt = m_Plugins.rbegin(); rIt != m_Plugins.rend(); rIt++)
	{
		auto& plugin = (*rIt);
		try
		{
			plugin.Plugin.reset();

			applicationPluginSpecific.UnloadPlugin(plugin.Filename);
		}
		catch (const CException& e)
		{
			Log::Error("Error while removing plugin '%s'.", plugin.Plugin->GetName().c_str());
			Log::Error("--->%s", e.MessageCStr());
		}
	}
}

std::vector<std::shared_ptr<IznPlugin>> CPluginsManager::GetAllPlugins() const
{
	std::vector<std::shared_ptr<IznPlugin>> plugins;
	for (const auto& it : m_Plugins)
		plugins.push_back(it.Plugin);
	return plugins;
}

void CPluginsManager::AddPluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{
	if (PluginEventListener == nullptr)
		throw CException("PluginsManager: PluginEventListener is nullptr.");
	m_PluginsEventsListener.push_back(PluginEventListener);
}

void CPluginsManager::RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener)
{
	const auto& iter = std::find(m_PluginsEventsListener.begin(), m_PluginsEventsListener.end(), PluginEventListener);
	if (iter == m_PluginsEventsListener.end())
	{
		Log::Warn("Unable to remove plugin event listeneer.");
		return;
	}

	m_PluginsEventsListener.erase(iter);
}
