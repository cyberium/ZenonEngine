#pragma once

// Forward BEGIN
struct IManager;
struct IBaseManager;
// Forward END


struct OW_ENGINE_API IznPlugin
{
	virtual ~IznPlugin() {}

	virtual bool									Initialize(std::shared_ptr<IBaseManager> BaseManager) = 0;
	virtual void									Finalize() = 0;
};

struct OW_ENGINE_API
	__declspec(uuid("439F9EB6-D1F1-461C-B63B-A0D8CCAE0E58"))
	IznPluginsManager : public IManager
{
	virtual ~IznPluginsManager() {}

	virtual bool									RegisterPlugin(const std::string& PluginDLLName) = 0;
	virtual void									UnregisterPlugin() = 0;
};