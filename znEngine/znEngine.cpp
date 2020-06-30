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
#include "SceneFunctional/SceneNodeEngineCreator.h"

// Additional (Images)
#include "Formats/Images/ImagesFactory.h"
#include "Formats/Images/ImageLoaderTemplate.h"
#include "Formats/Images/ImagePNG.h"
#include "Formats/Images/ImageDDS.h"

#include "Settings/GroupVideo.h"


#include "ThreadPool.h"

// Additional
#include <filesystem>

namespace fs = std::experimental::filesystem;

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

std::string str_tolower(std::string s) 
{
	std::transform(s.begin(), s.end(), s.begin(), [] (char c) 
	{ 
		return std::tolower(c, std::locale()); }
	);
	return s;
}

std::vector<std::string> GetAllFilesInDirectory(const std::string& Directory, const std::vector<std::string> DirSkipList = { }, const std::string& FileExtention = "")
{
	std::vector<std::string> listOfFiles;

	try
	{
		// Check if given path exists and points to a directory
		if (fs::exists(Directory) && fs::is_directory(Directory))
		{
			// Create a Recursive Directory Iterator object and points to the starting of directory
			fs::recursive_directory_iterator iter(Directory);

			// Create a Recursive Directory Iterator object pointing to end.
			fs::recursive_directory_iterator end;

			// Iterate till end
			while (iter != end)
			{
				// Check if current entry is a directory and if exists in skip list
				if (fs::is_directory(iter->path()) && (std::find(DirSkipList.begin(), DirSkipList.end(), iter->path().filename()) != DirSkipList.end()))
				{
					iter.disable_recursion_pending();
				}
				else if (!fs::is_directory(iter->path()) && (!FileExtention.empty()) && (iter->path().has_extension()) && (str_tolower(iter->path().extension().string()) == str_tolower(FileExtention)))
				{
					listOfFiles.push_back(iter->path().string());
				}
				else
				{
					iter.disable_recursion_pending();
				}

				std::error_code ec;
				// Increment the iterator to point to next entry in recursive iteration
				iter.increment(ec);
				if (ec)
				{
					Log::Error("GetAllFilesInDirectory: Error while accessing '%s'. Error: '%s'.", iter->path().string().c_str(), ec.message().c_str());
				}
			}
		}
	}
	catch (const std::system_error& e)
	{
		Log::Error("GetAllFilesInDirectory: Exception '%s'", e.what());
	}

	return listOfFiles;
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

		std::shared_ptr<CSceneNode3DFactory> sceneNode3DFactory = std::make_shared<CSceneNode3DFactory>(*baseManager, ofkSceneNode3D);
		sceneNode3DFactory->AddClassCreator(std::make_shared<CSceneNode3DEngineCreator>(*baseManager));

		std::shared_ptr<CSceneNodeUIFactory> sceneNodeUIFactory = std::make_shared<CSceneNodeUIFactory>(*baseManager, ofkSceneNodeUI);
		sceneNodeUIFactory->AddClassCreator(std::make_shared<CSceneNodeUIEngineCreator>(*baseManager));

		factory->AddClassFactory(sceneNode3DFactory);
		factory->AddClassFactory(sceneNodeUIFactory);
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

			std::vector<std::string> fileNamesInWorkDirectory = GetAllFilesInDirectory(PathToPlugins, {}, ".dll");
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
