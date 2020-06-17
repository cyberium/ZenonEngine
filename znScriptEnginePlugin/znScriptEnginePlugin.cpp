#include "stdafx.h"

// General
#include "znScriptEnginePlugin.h"

IznPlugin* plugin = nullptr;

class CznScriptEnginePlugin : public IznPlugin
{
public:
	CznScriptEnginePlugin(IBaseManager* BaseManager)
	{
		m_BaseManager = BaseManager;
	}
	virtual ~CznScriptEnginePlugin()
	{

	}



	//
	// IznPlugin
	//
	bool Initialize()
	{
		

		return true;
	}
	void Finalize()
	{

	}

	std::string GetName() const override
	{
		return "Lua script plugin";
	}
	std::string GetDescription() const override
	{
		return "Provide lua support";
	}

private:
	IBaseManager* m_BaseManager;
};

IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznScriptEnginePlugin(BaseManager);
	}

	return plugin;
}
