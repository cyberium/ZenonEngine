#include "stdafx.h"

// General
#include "znFBXPlugin.h"

class CznFBXPlugin : public IznPlugin
{
public:
	CznFBXPlugin()
	{

	}
	virtual ~CznFBXPlugin()
	{

	}



	//
	// IznPlugin
	//
	bool Initialize(std::shared_ptr<IBaseManager> BaseManager)
	{
		m_BaseManager = BaseManager;

		return false;
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
		plugin = new CznFBXPlugin();
		plugin->Initialize(BaseManager);
	}

	return plugin;
}
