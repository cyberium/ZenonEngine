#pragma once

class CPluginException : public std::exception
{
public:
	CPluginException(const std::string& Message)
		: std::exception(Message.c_str())
	{}
	virtual ~CPluginException()
	{}
};

class CPluginException_NotAPlguin : public CPluginException
{
public:
	CPluginException_NotAPlguin(const std::string& Message)
		: CPluginException(Message)
	{}
	virtual ~CPluginException_NotAPlguin()
	{}
};

class OW_ENGINE_API CznPluginsManager : public IznPluginsManager
{
public:
	CznPluginsManager(IBaseManager* BaseManager);
	virtual ~CznPluginsManager();

	// IznPluginsManager
	bool											RegisterPlugin(const std::string& PluginDLLName);
	void											UnregisterPlugin();

private:
	IBaseManager*					m_BaseManager;
	std::map<std::string, std::shared_ptr<IznPlugin>> m_Plugins;
};

