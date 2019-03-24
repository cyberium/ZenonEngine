#include "stdafx.h"

// Additional (TEST)
#include "GameState_World.h"

// Additional (Windows)
#include <ctime>

int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	{
		_BaseManager = std::make_shared<CBaseManager>();

		std::shared_ptr<CSettings> settings = std::make_shared<CSettings>();
		AddManager<ISettings>(settings);
		settings->AddDefaults();
		

		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		AddManager<ILog>(log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>();
		AddManager<IConsole>(console);
		console->AddCommonCommands();
		
		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>();
		AddManager<IFilesManager>(filesManager);

		std::shared_ptr<IFilesStorage> localFilesGamedata = std::make_shared<CLocalFilesStorage>("D:\\_programming\\OpenWoW\\_gamedata\\");
		filesManager->RegisterFilesStorage(localFilesGamedata);


        HMODULE m_HINSTANCE = ::GetModuleHandle(NULL);

        CWindowObject* windowObject = new CWindowObject();
        windowObject->RegisterWindowClass(m_HINSTANCE);
        windowObject->CreateWindowInstance(1280, 1024);

		//--

		Application app;

        std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice();
        std::shared_ptr<RenderWindow> renderWindow = app.CreateRenderWindow(windowObject, true);


        std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>();
        AddManager<IFontsManager>(fontsManager);

		app.AddGameState(GameStatesNames::GAME_STATE_WORLD, std::make_shared<CGameState_World>(&app));
		app.SetGameState(GameStatesNames::GAME_STATE_WORLD);
		app.Run();
	}

	return 0;
}
