#include "stdafx.h"

// General
#include "znEngine.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"
#include "Settings.h"
#include "RenderDeviceFactory.h"
#include "MaterialsFactory.h"
#include "Loader.h"
#include "Passes/RenderPassFactory.h"
#include "SceneFunctional/ScenesFactory.h"
#include "SceneFunctional/ComponentsEngineCreator.h"
#include "SceneFunctional/SceneNodeEngineCreator.h"
#include "FBX/FBXManager.h"

// Additional (Images)
#include "Formats/Images/ImagesFactory.h"
#include "Formats/Images/ImageLoaderTemplate.h"
#include "Formats/Images/ImagePNG.h"
#include "Formats/Images/ImageDDS.h"

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
	IBaseManager* baseManager = new CBaseManager();

	std::shared_ptr<IznPluginsManager> pluginsManager = std::make_shared<CznPluginsManager>(*baseManager);
	baseManager->AddManager<IznPluginsManager>(pluginsManager);

	// Settings
	{
		std::shared_ptr<ISettings> settings = std::make_shared<CSettings>(*baseManager);
		baseManager->AddManager<ISettings>(settings);
		settings->AddGroup("Video", std::make_shared<CGroupVideo>());
	}

	// Files
	{
		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>(*baseManager);
		baseManager->AddManager<IFilesManager>(filesManager);
		filesManager->AddFilesStorage("ModuleFS", std::make_shared<CLibraryResourceFileStotage>(GetModuleHandle(L"znEngine.dll")));
		filesManager->AddFilesStorage("PCEveryFileAccess", std::make_shared<CLocalFilesStorage>(""));
		filesManager->AddFilesStorage("ZenonGamedata", std::make_shared<CLocalFilesStorage>("C:\\_engine\\ZenonEngine_gamedata\\"));
		filesManager->AddFilesStorage("OpenWOWGamedata", std::make_shared<CLocalFilesStorage>("C:\\_engine\\OpenWoW\\_gamedata\\"));
	}

	// Log & console
	{
		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		baseManager->AddManager<ILog>(log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>(*baseManager);
		baseManager->AddManager<IConsole>(console);
		console->AddCommonCommands();
	}

	// Render stuff
	{
		baseManager->AddManager<IImagesFactory>(std::make_shared<CImagesFactory>(*baseManager));
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(std::make_shared<CImageLoaderT<CImagePNG>>());
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(std::make_shared<CImageLoaderT<CImageDDS>>());

		auto materialsFactory = std::make_shared<CMaterialsFactory>(*baseManager);
		baseManager->AddManager<IMaterialsFactory>(materialsFactory);

		std::shared_ptr<IznRenderDeviceFactory> renderDeviceFactory = std::make_shared<CznRenderDeviceFactory>(*baseManager);
		baseManager->AddManager<IznRenderDeviceFactory>(renderDeviceFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(renderDeviceFactory));
	}

	// SceneNodes stuff
	{
		std::shared_ptr<ILoader> laoder = std::make_shared<CLoader>();
		baseManager->AddManager<ILoader>(laoder);

		std::shared_ptr<IObjectsFactory> factory = std::make_shared<CObjectsFactory>(*baseManager);
		baseManager->AddManager<IObjectsFactory>(factory);

		std::shared_ptr<CSceneNode3DFactory> sceneNode3DFactory = std::make_shared<CSceneNode3DFactory>(*baseManager, "otSceneNode3D", otSceneNode3D);
		sceneNode3DFactory->AddClassCreator(std::make_shared<CSceneNode3DEngineCreator>(*baseManager));

		std::shared_ptr<CSceneNodeUIFactory> sceneNodeUIFactory = std::make_shared<CSceneNodeUIFactory>(*baseManager, "otSceneNodeUI", otSceneNodeUI);
		sceneNodeUIFactory->AddClassCreator(std::make_shared<CSceneNodeUIEngineCreator>(*baseManager));

		std::shared_ptr<CComponentsFactory> componentFactory = std::make_shared<CComponentsFactory>(*baseManager, "otSceneNodeComponent", otSceneNodeComponent);
		componentFactory->AddClassCreator(std::make_shared<CComponentsEngineCreator>(*baseManager));


		factory->AddClassFactory(sceneNode3DFactory);
		factory->AddClassFactory(sceneNodeUIFactory);
		factory->AddClassFactory(componentFactory);
	}

	// FBX
	{
		auto fbxManager = std::make_shared<CFBXManager>(*baseManager);
		baseManager->AddManager<IFBXManager>(fbxManager);
	}

	// Scene
	{
		std::shared_ptr<IScenesFactory> factory = std::make_shared<CScenesFactory>(*baseManager);
		baseManager->AddManager<IScenesFactory>(factory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(factory));
	}

	// Passes
	{
		std::shared_ptr<IRenderPassFactory> factory = std::make_shared<CRenderPassFactory>();
		baseManager->AddManager<IRenderPassFactory>(factory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(factory));
	}

	// Plugins
	{
		try
		{
			if (PathToPlugins.empty())
				PathToPlugins = GetExePath();

			std::vector<std::string> fileNamesInWorkDirectory = Utils::GetAllFilesInDirectory(PathToPlugins, ".dll");
			for (const auto& it : fileNamesInWorkDirectory)
			{
				//if (it.find("znRender") != std::string::npos)
					pluginsManager->AddPlugin(it);
			}
		}
		catch (const std::exception& e)
		{
			Log::Error("Error while initialize plugin: %s", e.what());
		}

		pluginsManager->InitializeAllPlugins();
	}

	return baseManager;
}
