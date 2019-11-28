#pragma once

class OW_ENGINE_API CznPluginsManager : public IznPluginsManager
{
public:
	CznPluginsManager();
	virtual ~CznPluginsManager();

	// IznPluginsManager
	bool											RegisterPlugin(const std::string& PluginDLLName);
	void											UnregisterPlugin();

private:
	std::map<std::string, std::shared_ptr<IznPlugin>> m_Plugins;
};

