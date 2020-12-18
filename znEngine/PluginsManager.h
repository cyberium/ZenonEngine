#pragma once

class ZN_API CPluginsManager
	: public IznPluginsManager
{
public:
	CPluginsManager(IApplication& Application);
	virtual ~CPluginsManager();

	// IznPluginsManager
	bool											AddPlugin(const std::string& PluginDLLName) override;
	void											RemovePlugin(std::shared_ptr<IznPlugin> Plugin) override;

	void											InitializeAllPlugins() override;
	void											FinalizeAllPlugins() override;
	void									        RemoveAllPlugins() override;

	std::vector<std::shared_ptr<IznPlugin>>         GetAllPlugins() const override;

	void                                            AddPluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener) override;
	void                                            RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener) override;

private:
	struct SPluginDescription
	{
		std::string Filename;
		std::shared_ptr<IznPlugin> Plugin;
	};

private:
	IApplication& m_Application;
	std::vector<SPluginDescription> m_Plugins;
	std::vector<std::shared_ptr<IznPluginsEventListener>> m_PluginsEventsListener;
};

