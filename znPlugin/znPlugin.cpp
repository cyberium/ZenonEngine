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


IznPlugin* plugin = nullptr;
IznPlugin* GetPlugin(std::shared_ptr<IBaseManager> BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznTestPlugin();
	}

	return plugin;
}
