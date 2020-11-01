#include "stdafx.h"

// General
#include "znEngine.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"
#include "Settings.h"
#include "RenderDeviceFactory.h"
#include "AssyncLoader/Loader.h"

#include "Scene/Factory/SceneCreator.h"
#include "Scene/Factory/ComponentsCreator.h"
#include "Scene/Factory/SceneNodeCreator.h"


// Additional (Images)
#include "Formats/Images/ImagesFactory.h"
#include "Formats/Images/ImageLoaderTemplate.h"
#include "Formats/Images/ImagePNG.h"
#include "Formats/Images/ImageDDS.h"


// Additional (Models)
#include "Formats/Models/ModelsFactory.h"
#include "Formats/Models/EngineModelsLoader.h"
#include "Formats/Models/FBXModelsLoader.h"


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

std::string GetDLLPath()
{
	return GetExePath() + "\\Extensions\\";
}

#include <iostream>

IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins)
{
	IBaseManager* baseManager = ZN_NEW CBaseManager();

	// Log & console
	{
		baseManager->AddManager<ILog>(MakeShared(CLog));

		std::shared_ptr<CConsole> console = MakeShared(CConsole, *baseManager);
		baseManager->AddManager<IConsole>(console);
		console->AddCommonCommands();
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
		std::shared_ptr<IFilesManager> filesManager = MakeShared(CFilesManager, *baseManager);
		baseManager->AddManager<IFilesManager>(filesManager);
		filesManager->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLocalFilesStorage, "O:/ZenonEngine_gamedata/"));
		filesManager->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znEngine.dll")));
		filesManager->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, ""));
		filesManager->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "O:/ZenonEngine/gamedata/"));
		
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
		baseManager->GetManager<IznModelsFactory>()->AddModelsLoader(MakeShared(CznFBXModelsLoader, *baseManager));
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

		std::shared_ptr<CScenesFactory> sceneFactory = MakeShared(CScenesFactory, *baseManager, "otScene", otScene);
		sceneFactory->AddClassCreator(MakeShared(CSceneEngineCreator, *baseManager));
		factory->AddClassFactory(sceneFactory);

		std::shared_ptr<CSceneNode3DFactory> sceneNode3DFactory = MakeShared(CSceneNode3DFactory, *baseManager, "otSceneNode3D", otSceneNode3D);
		sceneNode3DFactory->AddClassCreator(MakeShared(CSceneNode3DEngineCreator, *baseManager));
		factory->AddClassFactory(sceneNode3DFactory);

		std::shared_ptr<CSceneNodeUIFactory> sceneNodeUIFactory = MakeShared(CSceneNodeUIFactory, *baseManager, "otSceneNodeUI", otSceneNodeUI);
		sceneNodeUIFactory->AddClassCreator(MakeShared(CSceneNodeUIEngineCreator, *baseManager));
		factory->AddClassFactory(sceneNodeUIFactory);

		std::shared_ptr<CComponentsFactory> componentFactory = MakeShared(CComponentsFactory, *baseManager, "otSceneNodeComponent", otSceneNodeComponent);
		componentFactory->AddClassCreator(MakeShared(CComponentsEngineCreator, *baseManager));
		factory->AddClassFactory(componentFactory);
	}

	// Plugins
	{
		try
		{
			if (PathToPlugins.empty())
				PathToPlugins = GetExePath();

			pluginsManager->AddPlugin(PathToPlugins + "\\" + "znRenderDX11.dll");
		}
		catch (const std::exception& e)
		{
			Log::Error("Error while initialize plugin: %s", e.what());
		}

		pluginsManager->InitializeAllPlugins();
	}

	return baseManager;
}
