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
	bool Initialize(std::shared_ptr<IBaseManager> BaseManager)
	{
		m_BaseManager = BaseManager;

		return true;
	}
	void Finalize()
	{

	}

private:
	std::shared_ptr<IBaseManager> m_BaseManager;
};

IznPlugin* GetPlugin(std::shared_ptr<IBaseManager> BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznScriptEnginePlugin();
	}

	return plugin;
}
