#include "stdafx.h"

// General
#include "znPlugin.h"

std::shared_ptr<IBaseManager> _BaseManager = nullptr;
IznPlugin* plugin = nullptr;

class CznTestPlugin : public IznPlugin
{
public:
	CznTestPlugin()
	{

	}
	virtual ~CznTestPlugin() 
	{

	}



	//
	// IznPlugin
	//
	bool Initialize(const std::shared_ptr<IBaseManager>& BaseManager)
	{
		return false;
	}
	void Finalize()
	{

	}

private:

};

IznPlugin* GetPlugin(std::shared_ptr<IBaseManager> BaseManager)
{
	_BaseManager = BaseManager;

	if (plugin == nullptr)
	{
		plugin = new CznTestPlugin();
		plugin->Initialize(BaseManager);
	}

	return plugin;
}
