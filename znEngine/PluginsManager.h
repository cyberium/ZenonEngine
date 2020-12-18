#pragma once

class ZN_API CPluginsManager
	: public IznPluginsManager
{
public:
	CPluginsManager(IBaseManager& BaseManager);
	virtual ~CPluginsManager();

	// IznPluginsManager
	bool											AddPlugin(const std::string& PluginDLLName) override;
	void											RemovePlugin(const std::string& PluginDLLName) override;
	void											InitializeAllPlugins() override;
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
	IBaseManager& m_BaseManager;
	std::vector<SPluginDescription> m_Plugins;
	std::vector<std::shared_ptr<IznPluginsEventListener>> m_PluginsEventsListener;
};

