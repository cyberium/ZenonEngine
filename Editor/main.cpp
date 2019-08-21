#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QtWidgets/QApplication>

#include "RenderWindowWidget.h"


#include "GameState_World.h"

int main(int argc, char *argv[])
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

	HMODULE hModule = GetModuleHandle(NULL);
	std::shared_ptr<IFilesStorage> libraryFileStorage = std::make_shared<CLibraryResourceFileStotage>(hModule);
	filesManager->RegisterFilesStorage(libraryFileStorage);


	//--


	QApplication a(argc, argv);
	MainEditor w;


	Application app(_BaseManager);
	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(IRenderDevice::DeviceType::OpenGL);
	std::shared_ptr<RenderWindow> renderWindow = app.CreateRenderWindow(dynamic_cast<IWindowObject*>(w.getUI().frame), true);

	dynamic_cast<RenderWindowWidget*>(w.getUI().frame)->SetRenderWindow(renderWindow);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>();
	AddManager<IFontsManager>(fontsManager);

	app.AddGameState(GameStatesNames::GAME_STATE_WORLD, std::make_shared<CGameState_World>(&app));
	app.SetGameState(GameStatesNames::GAME_STATE_WORLD);

	w.ApplyScene(std::dynamic_pointer_cast<Scene3D, Scene>(app.GetGameState()->GetScene()));
	
	w.show();

	app.Run();

	return a.exec();
}
