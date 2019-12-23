#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QtWidgets/QApplication>

#include "RenderWindowWidget.h"

int main(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argc, argv));

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor w;

	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	AddManager<IRenderDevice>(BaseManager, renderDevice);

	std::shared_ptr<IRenderWindow> renderWindow = renderDevice->CreateRenderWindow(dynamic_cast<IWindowObject*>(w.getUI().EditorWindow), false);
	app.AddRenderWindow(renderWindow);

	w.getUI().EditorWindow->SetRenderWindow(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindow));

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, BaseManager);
	AddManager<IFontsManager>(BaseManager, fontsManager);

	std::shared_ptr<IGameState> gameState = GetManager<IGameStatesFactory>(BaseManager)->CreateGameStateWithHighestPriority(renderWindow);
	app.SetGameState(gameState);

	w.ApplyScene(std::dynamic_pointer_cast<IScene>(app.GetGameState()->GetScene3D()));
	
	w.show();

	app.Run();

	return a.exec();
}
