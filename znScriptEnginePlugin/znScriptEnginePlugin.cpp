#include "stdafx.h"

// General
#include "znScriptEnginePlugin.h"

IznPlugin* plugin = nullptr;

class CznScriptEnginePlugin : public IznPlugin
{
public:
	CznScriptEnginePlugin()
	{

	}
	virtual ~CznScriptEnginePlugin()
	{

	}



	//
	// IznPlugin
	//
	bool Initialize(IBaseManager* BaseManager)
	{
		m_BaseManager = BaseManager;

		return true;
	}
	void Finalize()
	{

	}

private:
	IBaseManager* m_BaseManager;
};

IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznScriptEnginePlugin();
	}

	return plugin;
}
