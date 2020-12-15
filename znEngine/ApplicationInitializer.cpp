#include "stdafx.h"

// General
#include "Application.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"

#include "Settings/Settings.h"

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
#include "Formats/Models/ModelsLoader_znmdl.h"
#include "Formats/Models/ModelsLoader_znxmdl.h"

// Additional (Materials)
#include "Formats/Materials/MaterialsFactory.h"


// Additional (Textures)
#include "Formats/Textures/TexturesFactory.h"


#include "ThreadPool.h"



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


	// Files
	{
		GetBaseManager().AddManager<IFilesManager>(MakeShared(CFilesManager, GetBaseManager()));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::USERDATA, MakeShared(CLocalFilesStorage, "O:/ZenonEngine_userdata/"));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLocalFilesStorage, "O:/ZenonEngine_assets/"));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znEngine.dll")));
		GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLocalFilesStorage, "gamedata/"));
	}


	// Settings
	{
		GetBaseManager().AddManager<ISettings>(MakeShared(CSettings, GetBaseManager()));

		// Video
		{
			auto settingsVideo = MakeShared(CPropertiesGroup, "Video", "Graphics settings.");
			settingsVideo->AddProperty(MakeShared(CProperty<glm::vec2>, "WindowSize", "Size of render windows", glm::vec2(600, 480)));
			settingsVideo->AddProperty(MakeShared(CProperty<bool>, "VSyncEnabled", "Enable or disable vertical synchronization", true));
			settingsVideo->AddProperty(MakeShared(CProperty<bool>, "MultisampleEnabled", "Enable or disable multisampling.", true));

			settingsVideo->GetPropertyT<glm::vec2>("WindowSize")->Set(glm::vec2(1280, 1024));
			settingsVideo->GetPropertyT<bool>("VSyncEnabled")->Set(false);
			settingsVideo->GetPropertyT<bool>("MultisampleEnabled")->Set(false);

			GetBaseManager().GetManager<ISettings>()->AddGroup(settingsVideo);
		}

		// Files
		{
			auto settingsFilesystem = MakeShared(CPropertiesGroup, "FileSystem", "descr");
			settingsFilesystem->AddProperty(MakeShared(CProperty<std::string>, "UserdataPath", "desc", "userdata/"));

			settingsFilesystem->GetPropertyT<std::string>("UserdataPath")->Set("O:/ZenonEngine_userdata/");

			GetBaseManager().GetManager<ISettings>()->AddGroup(settingsFilesystem);
		}

		GetBaseManager().GetManager<ISettings>()->Load();
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
		GetBaseManager().GetManager<IznModelsFactory>()->AddModelsLoader(MakeShared(CModelsLoader_znmdl, GetBaseManager()));
		GetBaseManager().GetManager<IznModelsFactory>()->AddModelsLoader(MakeShared(CModelsLoader_znxmdl, GetBaseManager()));
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
				pluginsManager->AddPlugin(Utils::GetExecutablePath() + "\\" + p);
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
