#include "stdafx.h"

// General
#include "znEngine.h"

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

std::string GetExeFileName()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	return Resources::utf16_to_utf8(buffer);
}

std::string GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
}

#include <iostream>

IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins)
{
	IBaseManager* baseManager = ZN_NEW CBaseManager();

	Random::Initialize();

	// Log & console
	{
		baseManager->AddManager<ILog>(MakeShared(CLog));

		std::shared_ptr<CConsole> console = MakeShared(CConsole, *baseManager);
		baseManager->AddManager<IConsole>(console);
	}


	std::shared_ptr<IznPluginsManager> pluginsManager = MakeShared(CznPluginsManager, *baseManager);
	baseManager->AddManager<IznPluginsManager>(pluginsManager);


	// Settings
	{
		std::shared_ptr<ISettings> settings = MakeShared(CSettings, *baseManager);
		baseManager->AddManager<ISettings>(settings);
		settings->AddGroup("Video", MakeShared(CGroupVideo));
	}


	// Files
	{
		baseManager->AddManager<IFilesManager>(MakeShared(CFilesManager, *baseManager));
		baseManager->GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA,   MakeShared(CLocalFilesStorage, "O:/ZenonEngine_gamedata/"));
		baseManager->GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znEngine.dll")));
		baseManager->GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "gamedata/"));
		baseManager->GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "")); // Every access
	}



	// Render stuff
	{
		std::shared_ptr<IznRenderDeviceFactory> renderDeviceFactory = MakeShared(CznRenderDeviceFactory, *baseManager);
		baseManager->AddManager<IznRenderDeviceFactory>(renderDeviceFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(renderDeviceFactory));
	}



	// Images
	{
		baseManager->AddManager<IImagesFactory>(MakeShared(CImagesFactory, *baseManager));
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImagePNG>));
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImageDDS>));
	}



	// Materials
	{
		baseManager->AddManager<IMaterialsFactory>(MakeShared(CMaterialsFactory, *baseManager));
	}



	// Models
	{
		baseManager->AddManager<IznModelsFactory>(MakeShared(CznModelsFactory, *baseManager));
		baseManager->GetManager<IznModelsFactory>()->AddModelsLoader(MakeShared(CznEngineModelsLoader, *baseManager));
	}



	// Textures
	{
		baseManager->AddManager<IznTexturesFactory>(MakeShared(CznTexturesFactory, *baseManager));
	}


	// SceneNodes stuff
	{
		baseManager->AddManager<ILoader>(MakeShared(CLoader));

		std::shared_ptr<IObjectsFactory> factory = MakeShared(CObjectsFactory, *baseManager);
		baseManager->AddManager<IObjectsFactory>(factory);

		std::shared_ptr<CSceneNodeFactory> sceneNodeFactory = MakeShared(CSceneNodeFactory, *baseManager, "otSceneNode", otSceneNode);
		factory->AddClassFactory(sceneNodeFactory);

		std::shared_ptr<CUIControlFactory> uiControlFactory = MakeShared(CUIControlFactory, *baseManager, "otUIControl", otUIControl);
		factory->AddClassFactory(uiControlFactory);

		std::shared_ptr<CComponentsFactory> componentFactory = MakeShared(CComponentsFactory, *baseManager, "otSceneNodeComponent", otSceneNodeComponent);
		factory->AddClassFactory(componentFactory);

		EngineSceneTypesExtender(*baseManager);
	}

	// Plugins
	{
		try
		{
			if (PathToPlugins.empty())
				PathToPlugins = GetExePath();

			pluginsManager->AddPlugin(PathToPlugins + "\\" + "znRenderDX11.dll");
			//pluginsManager->AddPlugin(PathToPlugins + "\\" + "znPluginM2Models.dll");
			pluginsManager->AddPlugin(PathToPlugins + "\\" + "znPluginFBXModels.dll");
		}
		catch (const std::exception& e)
		{
			Log::Error("Error while initialize plugin: %s", e.what());
		}

		pluginsManager->InitializeAllPlugins();
	}

	return baseManager;
}
