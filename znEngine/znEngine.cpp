#include "stdafx.h"

// General
#include "znEngine.h"

// Additional
#include "RenderDeviceCreatorFactory.h"

//
// CznEnginePlugin
//
class CznEnginePlugin : public IznPlugin
{
public:
	CznEnginePlugin()
	{}
	virtual ~CznEnginePlugin()
	{}

	// IznPlugin
	bool Initialize(std::shared_ptr<IBaseManager> BaseManager)
	{
		m_BaseManager = BaseManager;

		std::shared_ptr<CSettings> settings = std::make_shared<CSettings>(m_BaseManager);
		AddManager<ISettings>(m_BaseManager, settings);
		settings->AddDefaults();

		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		AddManager<ILog>(m_BaseManager, log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>(m_BaseManager);
		AddManager<IConsole>(m_BaseManager, console);
		console->AddCommonCommands();

		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>(m_BaseManager);
		AddManager<IFilesManager>(m_BaseManager, filesManager);

		std::shared_ptr<IznRenderDeviceCreatorFactory> renderDeviceCreatorFactory = std::make_shared<CznRenderDeviceCreatorFactory>();
		AddManager<IznRenderDeviceCreatorFactory>(m_BaseManager, renderDeviceCreatorFactory);

		return true;
	}
	void Finalize()
	{
		DelManager<IznRenderDeviceCreatorFactory>(m_BaseManager);
	}

private:
	std::shared_ptr<IBaseManager> m_BaseManager;
};

IznPlugin* plugin = nullptr;
IznPlugin* GetPlugin(std::shared_ptr<IBaseManager> BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznEnginePlugin();
		plugin->Initialize(BaseManager);
	}

	return plugin;
}