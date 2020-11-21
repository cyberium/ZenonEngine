#include "stdafx.h"

// General
#include "Application.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"
#include "Settings.h"
#include "AssyncLoader/Loader.h"


// Factories
#include "RenderDeviceFactory.h"
#include "Scene/EngineSceneExtender.h"


// Additional (Images)
#include "Formats/Images/ImagesFactory.h"
#include "Formats/Images/ImageLoaderTemplate.h"
#include "Formats/Images/ImagePNG.h"
#include "Formats/Images/ImageDDS.h"


// Additional (Models)
#include "Formats/Models/ModelsFactory.h"
#include "Formats/Models/EngineModelsLoader.h"


// Additional (Materials)
#include "Formats/Materials/MaterialsFactory.h"


// Additional (Textures)
#include "Formats/Textures/TexturesFactory.h"


#include "Settings/GroupVideo.h"

#include "ThreadPool.h"


namespace
{
	std::string GetExeFileName()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return buffer;
	}

	std::string GetExePath()
	{
		std::string f = GetExeFileName();
		return f.substr(0, f.find_last_of("\\/"));
	}
}


void Application::InitializeEngineInternal()
{
	m_BaseManager = std::make_unique<CBaseManager>();
	dynamic_cast<IBaseManagerInternal&>(GetBaseManager()).SetApplicationInternal(this);

	Random::Initialize();

	// Log & console
	{
		GetBaseManager().AddManager<ILog>(MakeShared(CLog));

		std::shared_ptr<CConsole> console = MakeShared(CConsole, GetBaseManager());
		GetBaseManager().AddManager<IConsole>(console);
	}


	std::shared_ptr<IznPluginsManager> pluginsManager = MakeShared(CznPluginsManager, GetBaseManager());
	GetBaseManager().AddManager<IznPluginsManager>(pluginsManager);


	// Settings
	{
		std::shared_ptr<ISettings> settings = MakeShared(CSettings, GetBaseManager());
		GetBaseManager().AddManager<ISettings>(settings);
		settings->AddGroup("Video", MakeShared(CGroupVideo));
	}


	// Files
	{
		GetBaseManager().AddManager<IFilesManager>(MakeShared(CFilesManager, GetBaseManager()));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLocalFilesStorage, "O:/ZenonEngine_gamedata/"));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znEngine.dll")));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "gamedata/"));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "")); // Every access
	}


	// Render stuff
	{
		std::shared_ptr<IznRenderDeviceFactory> renderDeviceFactory = MakeShared(CznRenderDeviceFactory, GetBaseManager());
		GetBaseManager().AddManager<IznRenderDeviceFactory>(renderDeviceFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(renderDeviceFactory));
	}


	// Images
	{
		GetBaseManager().AddManager<IImagesFactory>(MakeShared(CImagesFactory, GetBaseManager()));
		GetBaseManager().GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImagePNG>));
		GetBaseManager().GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImageDDS>));
	}


	// Materials
	{
		GetBaseManager().AddManager<IMaterialsFactory>(MakeShared(CMaterialsFactory, GetBaseManager()));
	}


	// Models
	{
		GetBaseManager().AddManager<IznModelsFactory>(MakeShared(CznModelsFactory, GetBaseManager()));
		GetBaseManager().GetManager<IznModelsFactory>()->AddModelsLoader(MakeShared(CznEngineModelsLoader, GetBaseManager()));
	}


	// Textures
	{
		GetBaseManager().AddManager<IznTexturesFactory>(MakeShared(CznTexturesFactory, GetBaseManager()));
	}


	// SceneNodes stuff
	{
		GetBaseManager().AddManager<ILoader>(MakeShared(CLoader));

		std::shared_ptr<IObjectsFactory> factory = MakeShared(CObjectsFactory, GetBaseManager());
		GetBaseManager().AddManager<IObjectsFactory>(factory);

		std::shared_ptr<CSceneNodeFactory> sceneNodeFactory = MakeShared(CSceneNodeFactory, GetBaseManager(), "otSceneNode", otSceneNode);
		factory->AddClassFactory(sceneNodeFactory);

		std::shared_ptr<CUIControlFactory> uiControlFactory = MakeShared(CUIControlFactory, GetBaseManager(), "otUIControl", otUIControl);
		factory->AddClassFactory(uiControlFactory);

		std::shared_ptr<CComponentsFactory> componentFactory = MakeShared(CComponentsFactory, GetBaseManager(), "otSceneNodeComponent", otSceneNodeComponent);
		factory->AddClassFactory(componentFactory);

		EngineSceneTypesExtender(GetBaseManager());
	}


	// Plugins
	{
		std::vector<std::string> plugins;
		plugins.push_back("znRenderDX11.dll");
		//plugins.push_back("znPluginM2Models.dll");
		plugins.push_back("znPluginFBXModels.dll");

		for (const auto& p : plugins)
		{
			try
			{
				pluginsManager->AddPlugin(GetExePath() + "\\" + p);
			}
			catch (const CException& e)
			{
				Log::Error("Error while initialize plugin '%s'.", p.c_str());
				Log::Error("--->%s", e.MessageCStr());
			}
		}

		pluginsManager->InitializeAllPlugins();
	}
}
