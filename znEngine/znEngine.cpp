#include "stdafx.h"

// General
#include "znEngine.h"

// Additional
#include "BaseManager.h"
#include "PluginsManager.h"
#include "RenderDeviceFactory.h"
#include "SceneFunctional/SceneNodesFactory.h"

#include "Settings/GroupVideo.h"


// Additional
#include <filesystem>

namespace fs = std::experimental::filesystem;

std::string GetExeFileName()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	return Resources::ConvertString(buffer);
}

std::string GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
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

IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments)
{
	IBaseManager* baseManager = new CBaseManager();

	std::shared_ptr<IznPluginsManager> pluginsManager = std::make_shared<CznPluginsManager>(baseManager);
	AddManager<IznPluginsManager>(baseManager, pluginsManager);

	// Settings
	{
		std::shared_ptr<ISettings> settings = std::make_shared<CSettings>(baseManager);
		AddManager<ISettings>(baseManager, settings);
		settings->AddGroup("Video", std::make_shared<CGroupVideo>());
	}

	// Files
	{
		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>(baseManager);
		AddManager<IFilesManager>(baseManager, filesManager);
		filesManager->RegisterFilesStorage(std::make_shared<CLibraryResourceFileStotage>(GetModuleHandle(L"znEngine.dll")));
	}

	// Log & console
	{
		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		AddManager<ILog>(baseManager, log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>(baseManager);
		AddManager<IConsole>(baseManager, console);
		console->AddCommonCommands();
	}

	// Render stuff
	{
		std::shared_ptr<IznRenderDeviceFactory> renderDeviceFactory = std::make_shared<CznRenderDeviceFactory>(baseManager);
		AddManager<IznRenderDeviceFactory>(baseManager, renderDeviceFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(renderDeviceFactory));
	}

	// SceneNodes stuff
	{
		std::shared_ptr<ISceneNodesFactory> sceneNodesFactory = std::make_shared<CSceneNodesFactory>();
		AddManager<ISceneNodesFactory>(baseManager, sceneNodesFactory);
		pluginsManager->AddPluginEventListener(std::dynamic_pointer_cast<IznPluginsEventListener>(sceneNodesFactory));
	}

	// Plugins
	{
		try
		{
			std::vector<std::string> fileNamesInWorkDirectory = GetAllFilesInDirectory(GetExePath(), {}, ".dll");
			for (const auto& it : fileNamesInWorkDirectory)
			{
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
