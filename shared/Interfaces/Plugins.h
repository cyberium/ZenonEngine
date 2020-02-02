#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE IBaseManager;
// Forward END


ZN_INTERFACE ZN_API IznPlugin
{
	virtual ~IznPlugin() {}

	virtual bool									Initialize() = 0;
	virtual void									Finalize() = 0;

	virtual std::string                             GetName() const = 0;
	virtual std::string                             GetDescription() const = 0;
};



ZN_INTERFACE ZN_API IznPluginsEventListener
{
	virtual ~IznPluginsEventListener() {}

	virtual void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin) = 0;
	virtual void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin) = 0;
	virtual void OnPluginFinalized(std::shared_ptr<IznPlugin> Plugin) = 0;
};



ZN_INTERFACE ZN_API
	__declspec(uuid("439F9EB6-D1F1-461C-B63B-A0D8CCAE0E58"))
	IznPluginsManager : public IManager
{
	virtual ~IznPluginsManager() {}

	virtual bool									AddPlugin(const std::string& PluginDLLName) = 0;
	virtual void									RemovePlugin(const std::string& PluginDLLName) = 0;

	virtual void                                    InitializeAllPlugins() = 0;

	virtual void                                    AddPluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener) = 0;
	virtual void                                    RemovePluginEventListener(std::shared_ptr<IznPluginsEventListener> PluginEventListener) = 0;
};
