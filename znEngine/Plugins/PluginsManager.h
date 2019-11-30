#pragma once

class OW_ENGINE_API CznPluginsManager : public IznPluginsManager
{
public:
	CznPluginsManager(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~CznPluginsManager();

	// IznPluginsManager
	bool											RegisterPlugin(const std::string& PluginDLLName);
	void											UnregisterPlugin();

private:
	std::shared_ptr<IBaseManager>					m_BaseManager;
	std::map<std::string, std::shared_ptr<IznPlugin>> m_Plugins;
};

