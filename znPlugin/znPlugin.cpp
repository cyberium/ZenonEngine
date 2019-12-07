#include "stdafx.h"

// General
#include "znPlugin.h"

class CznTestPlugin : public IznPlugin
{
public:
	CznTestPlugin()
	{}
	virtual ~CznTestPlugin() 
	{}



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


IznPlugin* plugin = nullptr;
IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznTestPlugin();
	}

	return plugin;
}
