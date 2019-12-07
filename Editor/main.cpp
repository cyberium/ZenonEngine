#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QtWidgets/QApplication>

#include "RenderWindowWidget.h"

int main(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argc, argv));

	// 2. Set file location
	GetManager<IFilesManager>(BaseManager)->RegisterFilesStorage(std::make_shared<CLocalFilesStorage>("D:\\_programming\\ZenonEngine\\gamedata\\"));

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor w;

	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	std::shared_ptr<IRenderWindow> renderWindow = renderDevice->CreateRenderWindow(dynamic_cast<IWindowObject*>(w.getUI().frame), false);
	app.AddRenderWindow(renderWindow);

	dynamic_cast<RenderWindowWidget*>(w.getUI().frame)->SetRenderWindow(renderWindow);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(BaseManager);
	AddManager<IFontsManager>(BaseManager, fontsManager);

	app.AddGameState(GameStatesNames::GAME_STATE_WORLD, std::make_shared<CGameState_World>(&app, renderWindow));
	app.SetGameState(GameStatesNames::GAME_STATE_WORLD);

	w.ApplyScene(std::dynamic_pointer_cast<IScene>(app.GetGameState()->GetScene3D()));
	
	w.show();

	app.Run();

	return a.exec();
}
