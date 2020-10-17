#include "stdafx.h"

// General
#include "znEngine.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"
#include "Settings.h"
#include "RenderDeviceFactory.h"
#include "MaterialsFactory.h"
#include "AssyncLoader/Loader.h"
#include "Passes/RenderPassFactory.h"
#include "Scene/Factory/SceneCreator.h"
#include "Scene/Factory/ComponentsCreator.h"
#include "Scene/Factory/SceneNodeCreator.h"
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
		filesManager->AddFilesStorage("ModuleFS", MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znEngine.dll")));
		filesManager->AddFilesStorage("PCEveryFileAccess", MakeShared(CLocalFilesStorage, ""));
		filesManager->AddFilesStorage("ZenonGamedata", MakeShared(CLocalFilesStorage, "O:/ZenonEngine/gamedata/"));
		filesManager->AddFilesStorage("ZenonGamedata2", MakeShared(CLocalFilesStorage, "O:/ZenonEngine_gamedata/"));
	}

	// Log & console
	{
		std::shared_ptr<CLog> log = MakeShared(CLog);
		baseManager->AddManager<ILog>(log);

		std::shared_ptr<CConsole> console = MakeShared(CConsole, *baseManager);
		baseManager->AddManager<IConsole>(console);
		console->AddCommonCommands();
	}

	// Render stuff
	{
		baseManager->AddManager<IImagesFactory>(MakeShared(CImagesFactory, *baseManager));
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImagePNG>));
		baseManager->GetManager<IImagesFactory>()->AddImageLoader(MakeShared(CImageLoaderT<CImageDDS>));

		auto materialsFactory = MakeShared(CMaterialsFactory, *baseManager);
		baseManager->AddManager<IMaterialsFactory>(materialsFactory);

		std::shared_ptr<IznRenderDeviceFactory> renderDeviceFactory = MakeShared(CznRenderDeviceFactory, *baseManager);
		baseManager->AddManager<IznRenderDeviceFactory>(renderDeviceFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(renderDeviceFactory));
	}

	// SceneNodes stuff
	{
		std::shared_ptr<ILoader> laoder = MakeShared(CLoader);
		baseManager->AddManager<ILoader>(laoder);

		std::shared_ptr<IObjectsFactory> factory = MakeShared(CObjectsFactory, *baseManager);
		baseManager->AddManager<IObjectsFactory>(factory);

		std::shared_ptr<CScenesFactory> sceneFactory = MakeShared(CScenesFactory, *baseManager, "otScene", otScene);
		sceneFactory->AddClassCreator(MakeShared(CSceneEngineCreator, *baseManager));

		std::shared_ptr<CSceneNode3DFactory> sceneNode3DFactory = MakeShared(CSceneNode3DFactory, *baseManager, "otSceneNode3D", otSceneNode3D);
		sceneNode3DFactory->AddClassCreator(MakeShared(CSceneNode3DEngineCreator, *baseManager));

		std::shared_ptr<CSceneNodeUIFactory> sceneNodeUIFactory = MakeShared(CSceneNodeUIFactory, *baseManager, "otSceneNodeUI", otSceneNodeUI);
		sceneNodeUIFactory->AddClassCreator(MakeShared(CSceneNodeUIEngineCreator, *baseManager));

		std::shared_ptr<CComponentsFactory> componentFactory = MakeShared(CComponentsFactory, *baseManager, "otSceneNodeComponent", otSceneNodeComponent);
		componentFactory->AddClassCreator(MakeShared(CComponentsEngineCreator, *baseManager));

		factory->AddClassFactory(sceneFactory);
		factory->AddClassFactory(sceneNode3DFactory);
		factory->AddClassFactory(sceneNodeUIFactory);
		factory->AddClassFactory(componentFactory);
	}

	// FBX
	{
		auto fbxManager = MakeShared(CFBXManager, *baseManager);
		baseManager->AddManager<IFBXManager>(fbxManager);
	}

	// Passes
	{
		std::shared_ptr<IRenderPassFactory> factory = MakeShared(CRenderPassFactory);
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
